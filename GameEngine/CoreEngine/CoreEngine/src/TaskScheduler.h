#pragma once

#include <functional>
#include <map>
#include <vector>
#include <set>
#include <limits>

#include "IdentifierHeap.h"

typedef std::function<void(float delta, float elapsed, int calls)> TimerCallback;

extern const int infiniteCalls;
extern const float infiniteTime;
extern const float eachFrame;
extern const float infinity;

namespace Engine
{
	class ObjectBase;
	class Object;
}

class EventBase
{
public:
	EventBase();
	~EventBase();

	int GetID() const { return ID; }

private:
	typedef IDHeap<EventBase*> EventHeap;

	static EventHeap Events;

	int ID = -1;
};

class Connection
{
public:
	~Connection();

	void Watch(const std::shared_ptr<Engine::ObjectBase>& handle);
	void Disconnect();
	void Disconnect(int timerID);
	void Disconnect(EventBase& event, int id = -1);
	void ConnectedTo(EventBase& event, int id);
	void ConnectedTo(int timerID);
	bool Check() const;
	virtual void Died() {}

private:
	typedef std::vector<std::weak_ptr<Engine::ObjectBase>> HandleVector;
	typedef std::vector<int> IDVector;
	typedef std::map<EventBase*, IDVector> EventMap;

	bool Enabled = true;
	HandleVector WatchList;
	EventMap ConnectedEvents;
	IDVector ConnectedTimers;
};

namespace Engine
{
	////Class_Base(Connection);
}

template <typename... Args>
class Event : public EventBase
{
public:
	typedef std::function<bool(Args...)> CallbackFunction;

	~Event();

	int Connect(const CallbackFunction& callback, const std::shared_ptr<Connection>& connection = nullptr);
	int ConnectWithObject(const CallbackFunction& callback, const std::shared_ptr<Engine::Object>& connector);
	void Fire(Args&&... args);

private:
	struct EventConnection
	{
		bool Connected = false;
		bool UsesConnection = true;
		std::weak_ptr<Connection> Connector;
		std::weak_ptr<Engine::Object> ConnectorObject;
		CallbackFunction Callback;
	};

	typedef IDHeap<EventConnection> ConnectionHeap;

	ConnectionHeap Connections;
};

class TaskTimer
{
public:
	int Calls = 0;
	int MaxCalls = 1;
	float Delay = 1;
	float LifeTime = 0;
	float NextCall = 0;
	float MaxTime = -1;
	int ID = 0;
	bool Connected = false;
	TimerCallback Callback;
	std::weak_ptr<Connection> Connector;
};

class TaskScheduler
{
public:
	static int Timer(float delay, const TimerCallback& callback, const std::shared_ptr<Connection>& connection = nullptr);
	static int Repeat(float delay, int calls, const TimerCallback& callback, const std::shared_ptr<Connection>& connection = nullptr);
	static int RepeatFor(float delay, float lifeTime, const TimerCallback& callback, const std::shared_ptr<Connection>& connection = nullptr);
	static int RepeatUntil(float delay, int calls, float lifeTime, const TimerCallback& callback, const std::shared_ptr<Connection>& connection = nullptr);
	static void KillTimer(int id);
	static void Update(float delta);

private:
	typedef std::vector<TaskTimer> TimerHeap;
	typedef IDHeap<int> TimerIDHeap;

	static bool RunningTimer;
	static TimerHeap TimerQueue;
	static TimerHeap NewTimers;
	static TimerIDHeap TimerIDs;
	static float Time;
	
	static void FlushTimerBuffer();
	static void Push(const TaskTimer& timer);
	static void BalanceNodeDown(int i);
	static void BalanceNodeUp(int i);
	static void Pop();
};

template <typename ...Args>
Event<Args...>::~Event()
{
	for (int i = 0; i < Connections.Size(); ++i)
	{
		if (!Connections.NodeAllocated(i))
			continue;

		EventConnection& node = Connections.GetNode(i).GetData();

		if (!node.Connector.expired())
			node.Connector.lock()->Disconnect(*this);
	}
}

template <typename ...Args>
int Event<Args...>::Connect(const CallbackFunction& callback, const std::shared_ptr<Connection>& connection)
{
	EventConnection config;

	config.Connected = connection != nullptr;
	config.Connector = connection;
	config.Callback = callback;

	int connectionID = Connections.RequestID(config);

	if (connection != nullptr)
		connection->ConnectedTo(*this, connectionID);

	return connectionID;
}

template <typename ...Args>
int Event<Args...>::ConnectWithObject(const CallbackFunction& callback, const std::shared_ptr<Engine::Object>& connector)
{
	EventConnection config;

	config.Connected = connector != nullptr;
	config.UsesConnection = false;
	config.ConnectorObject = connector;
	config.Callback = callback;

	int connectionID = Connections.RequestID(config);

	return connectionID;
}

template <typename ...Args>
void Event<Args...>::Fire(Args&&... args)
{
	for (int i = 0; i < Connections.Size(); ++i)
	{
		if (!Connections.NodeAllocated(i))
			continue;

		EventConnection& node = Connections.GetNode(i).GetData();

		if (node.Connected)
		{
			if (node.UsesConnection && (node.Connector.expired() || !node.Connector.lock()->Check()))
				Connections.Release(i);
			else if (!node.UsesConnection && node.ConnectorObject.expired())
				Connections.Release(i);
		}
		else
			if (!node.Callback(std::forward<Args>(args)...))
				Connections.Release(i);
	}
}