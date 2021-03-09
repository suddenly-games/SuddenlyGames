#include "LuaInput.h"

namespace Engine
{
	bool InputData::SetState(bool state, bool suppressChanged)
	{
		StateChanged = !suppressChanged && state != State;
		State = state;

		if (StateChanged)
			StateEnum = State ? Enum::InputState::Began : Enum::InputState::Ended;
		else
			StateEnum = State ? Enum::InputState::Active : Enum::InputState::Idle;

		return StateChanged;
	}

	bool InputData::SetPosition(const Vector3& position, bool suppressChanged)
	{
		Delta = position - Position;
		Position = position;

		StateChanged = !suppressChanged && Delta != Vector3();

		return StateChanged;
	}

	void InputObject::SetState(bool state, Enum::BoundDevice device, bool suppressChanged, const std::shared_ptr<InputObject>& input)
	{
		if (Inputs[device].SetState(state, suppressChanged))
		{
			std::shared_ptr<InputObject> object = input;

			if (object == nullptr)
				object = This.lock()->Cast<InputObject>();

			if (state)
				Began.Fire(object);
			else
				Ended.Fire(object);

			Changed.Fire(object);
		}
	}

	void InputObject::SetPosition(const Vector3& position, Enum::BoundDevice device, bool suppressChanged, const std::shared_ptr<InputObject>& input)
	{
		Inputs[device].SetPosition(position, suppressChanged);

		if (Inputs[device].GetStateChanged())
			Changed.Fire(input != nullptr ? input : This.lock()->Cast<InputObject>());
	}

	InputBinding::~InputBinding()
	{
		for (int i = 0; i < int(Bindings.size()); ++i)
		{
			Unbind(i);
			--i;
		}
	}

	void InputBinding::Update(float)
	{
		for (int i = 0; i < int(Bindings.size()); ++i)
		{
			if (Bindings[i].Input.expired())
			{
				Unbind(i);
				--i;
			}
		}
	}

	void InputBinding::Bind(const std::shared_ptr<InputObject>& input)
	{
		int beganConnection = -1;
		int changedConnection = -1;
		int endedConnection = -1;

		if (input->GetType() != Enum::InputType::Point)
		{
			beganConnection = input->Began.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				SetState(true, Find(object));

				return true;
			});

			endedConnection = input->Ended.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				SetState(false, Find(object));
				
				return true;
			});
		}
		else if (input->GetType() != Enum::InputType::Button)
			changedConnection = input->Ended.Connect([this](const std::shared_ptr<InputObject>& object)
			{
				SetPosition(object->GetPosition(), Find(object));

				return true;
			});

		Bindings.push_back(BindingData{ input->GetState(), beganConnection, changedConnection, endedConnection, input->GetDevice(), input });

		UpdateState(input->GetDevice());
	}

	void InputBinding::Unbind(const std::shared_ptr<InputObject>& input)
	{
		int index = Find(input);

		if (index == -1)
			return;

		Unbind(index);
	}

	int InputBinding::GetBindings() const
	{
		return int(Bindings.size());
	}

	std::shared_ptr<InputObject> InputBinding::GetBinding(int index) const
	{
		return Bindings[index].Input.lock();
	}

	bool InputBinding::IsBound(const std::shared_ptr<InputObject>& input) const
	{
		return Find(input) != -1;
	}

	typedef bool(*BindingModeCallback)(int active, int bound);

	BindingModeCallback ModeOperators[Enum::InputMode::Count] = {
		[](int active, int bound) -> bool
		{
			return active > 0;
		},
		[](int active, int bound) -> bool
		{
			return active == bound;
		},
		[](int active, int bound) -> bool
		{
			return active == 1;
		},
	};

	void InputBinding::SetState(bool state, int index, bool suppressUpdates)
	{
		Bindings[index].State = state;

		UpdateState(Bindings[index].Device, suppressUpdates);
	}

	void InputBinding::UpdateState(Enum::BoundDevice device, bool suppressUpdates)
	{
		int foundActive = 0;
		int foundBound = 0;
		int foundActiveAll = 0;

		for (int i = 0; i < int(Bindings.size()); ++i)
		{
			if (Bindings[i].Device == device)
			{
				++foundBound;

				if (Bindings[i].State)
					++foundActive;
			}

			if (Bindings[i].State)
				++foundActiveAll;
		}

		bool finalState = ModeOperators[BindingMode](foundActive, foundBound);
		bool finalStateAll = ModeOperators[BindingMode](foundActiveAll, int(Bindings.size()));

		InputObject::SetState(finalState, device, suppressUpdates);

		if (device != Enum::BoundDevice::Any)
			InputObject::SetState(finalStateAll, Enum::BoundDevice::Any, suppressUpdates);
	}

	void InputBinding::SetPosition(const Vector3& position, int index, bool suppressUpdates)
	{
		InputObject::SetPosition(position, Bindings[index].Device, suppressUpdates);

		if (Bindings[index].Device != Enum::BoundDevice::Any)
			InputObject::SetPosition(position, Bindings[index].Device, suppressUpdates);
	}

	void InputBinding::Unbind(int index)
	{
		if (!Bindings[index].Input.expired())
		{
			std::shared_ptr<InputObject> input = Bindings[index].Input.lock();

			if (Bindings[index].BeganConnection != -1)
				Connection().Disconnect(input->Began, Bindings[index].BeganConnection);

			if (Bindings[index].ChangedConnection != -1)
				Connection().Disconnect(input->Began, Bindings[index].ChangedConnection);

			if (Bindings[index].EndedConnection != -1)
				Connection().Disconnect(input->Began, Bindings[index].EndedConnection);
		}

		UpdateState(Bindings[index].Device);

		std::swap(Bindings[index], Bindings.back());

		Bindings.pop_back();
	}

	int InputBinding::Find(const std::shared_ptr<InputObject>& input) const
	{
		for (int i = 0; i < int(Bindings.size()); ++i)
			if (Bindings[i].Input.lock() == input)
				return i;

		return -1;
	}

	void InputDevice::Configure(::InputObject* input)
	{
		if (input == nullptr)
			throw std::string("Attempt to configure InputData with a null InputObject.");

		Input = input;

		Input->Began.ConnectWithObject([this] (::InputObject*) -> bool
		{
			SetState(true, GetDevice(), true);
			SetState(true, Enum::BoundDevice::Any);
		
			return true;
		}, This.lock()->Cast<Object>());
		
		Input->Changed.ConnectWithObject([this] (::InputObject* data) -> bool
		{
			if (GetType() == Enum::InputType::Button)
			{

				SetState(data->GetState(), GetDevice(), true);
				SetState(data->GetState(), Enum::BoundDevice::Any);
			}
			else
			{
				SetPosition(data->GetPosition(), GetDevice(), true);
				SetPosition(data->GetPosition(), Enum::BoundDevice::Any);
			}
		
			return true;
		}, This.lock()->Cast<Object>());
		
		Input->Ended.ConnectWithObject([this] (::InputObject*) -> bool
		{
			SetState(false, GetDevice(), true);
			SetState(false, Enum::BoundDevice::Any);
		
			return true;
		}, This.lock()->Cast<Object>());

		Name = Input->GetName();
	}

	Enum::InputType InputDevice::GetType()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetType: Attempt to use unconfigured InputData.");

		return Input->GetType();
	}

	Enum::InputCode InputDevice::GetCode()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetCode: Attempt to use unconfigured InputData.");

		return Input->GetCode();
	}

	Enum::BoundDevice InputDevice::GetDevice()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetCode: Attempt to use unconfigured InputData.");

		return Input->GetDevice();
	}

	std::shared_ptr<Object> InputDevice::CreateInput(::InputObject* input)
	{
		if (input == nullptr)
			throw std::string("Attempt to create InputData with a null InputObject.");

		std::shared_ptr<InputDevice> data = Engine::Create<InputDevice>();

		data->Configure(input);

		return data;
	}

	void UserInput::Configure(InputHandler* handler)
	{
		if (handler == nullptr)
			throw std::string("Attempt to configure UserInput with a null InputHandler.");

		Handler = handler;
	}

	bool UserInput::GetState(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetState: Attempt to use unconfigured UserInput.");

		return Handler->GetState(code);
	}

	bool UserInput::GetStateChanged(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetStateChanged: Attempt to use unconfigured UserInput.");

		return Handler->GetStateChanged(code);
	}

	Enum::InputState UserInput::GetStateEnum(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetStateChanged: Attempt to use unconfigured UserInput.");

		return Handler->GetStateEnum(code);
	}

	const Vector3& UserInput::GetPosition(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetPosition: Attempt to use unconfigured UserInput.");

		return Handler->GetPosition(code);
	}

	const Vector3& UserInput::GetDelta(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetDelta: Attempt to use unconfigured UserInput.");

		return Handler->GetDelta(code);
	}

	Enum::InputType UserInput::GetType(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetType: Attempt to use unconfigured UserInput.");

		return Handler->GetType(code);
	}

	const char* UserInput::GetName(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetName: Attempt to use unconfigured UserInput.");

		return Handler->GetName(code);
	}

	std::shared_ptr<InputDevice> UserInput::GetInput(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetInput: Attempt to use unconfigured UserInput.");

		while (code >= int(Inputs.size()))
		{
			int i = int(Inputs.size());

			auto input = InputDevice::CreateInput(&Handler->GetInput(Enum::InputCode(i)))->Cast<InputDevice>();

			input->SetParent(This.lock());

			Inputs.push_back(std::weak_ptr<InputDevice>());
			Inputs[i] = input;
		}

		return Inputs[code].lock();
	}
}
