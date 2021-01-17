#include "TaskScheduler.h"

bool TaskScheduler::RunningTimer = false;
typename TaskScheduler::TimerHeap TaskScheduler::TimerQueue = TimerHeap();
typename TaskScheduler::TimerHeap TaskScheduler::NewTimers = TimerHeap();
typename TaskScheduler::TimerIDHeap TaskScheduler::TimerIDs = TimerIDHeap();
float TaskScheduler::Time = 0;
typename EventBase::EventHeap EventBase::Events = EventHeap();

const int infiniteCalls = -1;
const float infiniteTime = -1;
const float eachFrame = 0;
const float infinity = std::numeric_limits<float>::max();

EventBase::EventBase()
{
	ID = Events.RequestID(this);
}

EventBase::~EventBase()
{
	Events.Release(ID);
}

Connection::~Connection()
{
	if (Enabled)
		Disconnect();
};

void Connection::Watch(const std::shared_ptr<Engine::ObjectBase>& handle)
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	WatchList.push_back(handle);
}

void Connection::Disconnect()
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	for (int i = 0; i < int(ConnectedTimers.size()); ++i)
		TaskScheduler::KillTimer(ConnectedTimers[i]);

	Enabled = false;
	Died();
}

void Connection::Disconnect(int timerID)
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	for (int i = 0; i < int(ConnectedTimers.size()); ++i)
	{
		if (ConnectedTimers[i] == timerID)
		{
			ConnectedTimers.erase(ConnectedTimers.begin() + i);

			break;
		}
	}
}

void Connection::Disconnect(EventBase& event, int id)
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	EventMap::iterator i = ConnectedEvents.find(&event);

	if (i != ConnectedEvents.end())
	{
		if (id == -1)
			ConnectedEvents.erase(i);
		else
		{
			for (int j = 0; j < int(i->second.size()); ++j)
			{
				if (i->second[j] == id)
				{
					i->second.erase(i->second.begin() + j);

					break;
				}
			}

			if (i->second.size() == 0)
				ConnectedEvents.erase(i);
		}
	}
}

void Connection::ConnectedTo(EventBase& event, int id)
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	ConnectedEvents[&event].push_back(id);
}

void Connection::ConnectedTo(int timerID)
{
	if (!Enabled)
		throw "Attempt to use dead connection";

	ConnectedTimers.push_back(timerID);
}

bool Connection::Check() const
{
	if (!Enabled)
		return false;

	for (int i = 0; i < int(WatchList.size()); ++i)
		if (WatchList[i].expired())
			return false;

	return true;
}

int TaskScheduler::Timer(float delay, const TimerCallback& callback, const std::shared_ptr<Connection>& connection)
{
	return RepeatUntil(delay, infiniteCalls, infiniteTime, callback, connection);
}

int TaskScheduler::Repeat(float delay, int calls, const TimerCallback& callback, const std::shared_ptr<Connection>& connection)
{
	return RepeatUntil(delay, calls, infiniteTime, callback, connection);
}

int TaskScheduler::RepeatFor(float delay, float lifeTime, const TimerCallback& callback, const std::shared_ptr<Connection>& connection)
{
	return RepeatUntil(delay, infiniteCalls, lifeTime, callback, connection);
}

int TaskScheduler::RepeatUntil(float delay, int calls, float lifeTime, const TimerCallback& callback, const std::shared_ptr<Connection>& connection)
{
	int id = TimerIDs.RequestID(-1);

	TaskTimer timer;

	timer.Delay = delay;
	timer.NextCall = Time + delay;
	timer.MaxCalls = calls;
	timer.Connected = connection != nullptr;
	timer.Connector = connection;
	timer.MaxTime = lifeTime;
	timer.Callback = callback;
	timer.ID = id;

	if (RunningTimer)
		NewTimers.push_back(timer);
	else
	{
		TimerIDs.GetNode(id).GetData() = int(TimerQueue.size());

		Push(timer);
	}

	return id;
}

void TaskScheduler::KillTimer(int id)
{
	if (id < 0 || id >= TimerIDs.Size() || !TimerIDs.NodeAllocated(id))
		throw "Attempt to kill timer with invalid ID";

	int& i = TimerIDs.GetNode(id).GetData();

	TimerQueue[i].NextCall = infinity;

	BalanceNodeUp(i);

	TimerQueue.pop_back();
	TimerIDs.Release(id);
}

void TaskScheduler::Update(float delta)
{
	Time += delta;

	while (TimerQueue.size() > 0 && TimerQueue[0].NextCall < Time)
	{
		bool cleanUp = TimerQueue[0].Connected && (TimerQueue[0].Connector.expired() || !TimerQueue[0].Connector.lock()->Check());

		if (!cleanUp)
		{
			++TimerQueue[0].Calls;

			float deltaTime = TimerQueue[0].Delay + (Time - TimerQueue[0].NextCall);

			TimerQueue[0].LifeTime += deltaTime;

			RunningTimer = true;

			TimerQueue[0].Callback(deltaTime, TimerQueue[0].LifeTime, TimerQueue[0].Calls);

			RunningTimer = false;

			FlushTimerBuffer();

			TimerQueue[0].NextCall += TimerQueue[0].Delay == 0 ? Time - TimerQueue[0].NextCall + 1e-5f : TimerQueue[0].Delay;

			cleanUp = TimerQueue[0].Calls == TimerQueue[0].MaxCalls || (TimerQueue[0].MaxTime > 0 && TimerQueue[0].LifeTime > TimerQueue[0].MaxTime);
		}

		if (cleanUp)
			Pop();
		else
			BalanceNodeUp(0);
	}
}

void TaskScheduler::FlushTimerBuffer()
{
	for (int i = 0; i < int(NewTimers.size()); ++i)
	{
		TimerIDs.GetNode(NewTimers[i].ID).GetData() = int(TimerQueue.size());

		Push(NewTimers[i]);
	}

	NewTimers.clear();
}

void TaskScheduler::Push(const TaskTimer& timer)
{
	TimerQueue.push_back(timer);

	BalanceNodeDown(int(TimerQueue.size()) - 1);
}

void TaskScheduler::BalanceNodeDown(int i)
{
	int parent = (i - 1) / 2;

	while (i > 0 && TimerQueue[parent].NextCall > TimerQueue[i].NextCall)
	{
		std::swap(
			TimerIDs.GetNode(TimerQueue[parent].ID).GetData(),
			TimerIDs.GetNode(TimerQueue[i].ID).GetData()
		);

		std::swap(TimerQueue[parent], TimerQueue[i]);

		i = parent;
		parent = (i - 1) / 2;
	}
}

void TaskScheduler::BalanceNodeUp(int i)
{
	bool swapped;

	do
	{
		int left = 2 * i + 1;
		int right = 2 * i + 2;
		swapped = false;

		float leftValue = infinity;
		float rightValue = infinity;

		if (left < int(TimerQueue.size()))
			leftValue = TimerQueue[left].NextCall;

		if (right < int(TimerQueue.size()))
			rightValue = TimerQueue[right].NextCall;

		if (leftValue < TimerQueue[i].NextCall && leftValue < rightValue)
		{
			std::swap(
				TimerIDs.GetNode(TimerQueue[left].ID).GetData(),
				TimerIDs.GetNode(TimerQueue[i].ID).GetData()
			);

			std::swap(TimerQueue[i], TimerQueue[left]);

			i = left;
			swapped = true;
		}
		else if (rightValue < TimerQueue[i].NextCall && leftValue >= rightValue)
		{
			std::swap(
				TimerIDs.GetNode(TimerQueue[right].ID).GetData(),
				TimerIDs.GetNode(TimerQueue[i].ID).GetData()
			);

			std::swap(TimerQueue[i], TimerQueue[right]);

			i = right;
			swapped = true;
		}
	} while (swapped);
}

void TaskScheduler::Pop()
{
	std::swap(
		TimerIDs.GetNode(TimerQueue[0].ID).GetData(),
		TimerIDs.GetNode(TimerQueue[int(TimerQueue.size()) - 1].ID).GetData()
	);

	std::swap(TimerQueue[0], TimerQueue[int(TimerQueue.size()) - 1]);
	TimerIDs.Release(TimerQueue[int(TimerQueue.size()) - 1].ID);
	TimerQueue.pop_back();

	if (TimerQueue.size() > 0)
		BalanceNodeUp(0);
}
