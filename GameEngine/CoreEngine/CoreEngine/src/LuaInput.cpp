#include "LuaInput.h"

namespace Engine
{
	void InputData::Initialize()
	{
	}

	void InputData::Configure(InputObject* input)
	{
		if (input == nullptr)
			throw std::string("Attempt to configure InputData with a null InputObject.");

		Input = input;

		Input->Began.ConnectWithObject([this] (InputObject*) -> bool
		{
			Began.Fire(This.lock()->Cast<InputData>());
		
			return true;
		}, This.lock()->Cast<Object>());
		
		Input->Changed.ConnectWithObject([this] (InputObject*) -> bool
		{
			Changed.Fire(This.lock()->Cast<InputData>());
		
			return true;
		}, This.lock()->Cast<Object>());
		
		Input->Ended.ConnectWithObject([this] (InputObject*) -> bool
		{
			Ended.Fire(This.lock()->Cast<InputData>());
		
			return true;
		}, This.lock()->Cast<Object>());
	}

	bool InputData::GetState()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetState: Attempt to use unconfigured InputData.");

		return Input->GetState();
	}

	bool InputData::GetStateChanged()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetStateChanged: Attempt to use unconfigured InputData.");

		return Input->GetStateChanged();
	}

	const Vector3& InputData::GetPosition()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetPosition: Attempt to use unconfigured InputData.");

		return Input->GetPosition();
	}

	const Vector3& InputData::GetDelta()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetDelta: Attempt to use unconfigured InputData.");

		return Input->GetDelta();
	}

	Enum::InputType InputData::GetType()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetType: Attempt to use unconfigured InputData.");

		return Input->GetType();
	}

	Enum::InputCode InputData::GetCode()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetCode: Attempt to use unconfigured InputData.");

		return Input->GetCode();
	}

	std::string InputData::GetName()
	{
		if (Input == nullptr)
			throw std::string("InputData.GetName: Attempt to use unconfigured InputData.");

		return Input->GetName();
	}

	std::shared_ptr<Object> InputData::CreateInput(InputObject* input)
	{
		if (input == nullptr)
			throw std::string("Attempt to create InputData with a null InputObject.");

		std::shared_ptr<InputData> data = CreateObject("InputData")->Cast<InputData>();

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

	std::shared_ptr<InputData> UserInput::GetInput(Enum::InputCode code)
	{
		if (Handler == nullptr)
			throw std::string("UserInput.GetInput: Attempt to use unconfigured UserInput.");

		while (code >= int(Inputs.size()))
		{
			int i = int(Inputs.size());

			auto input = InputData::CreateInput(&Handler->GetInput(Enum::InputCode(i)))->Cast<InputData>();

			input->Name = input->GetName();
			input->SetParent(This.lock());

			Inputs.push_back(std::weak_ptr<InputData>());
			Inputs[i] = input;
		}

		return Inputs[code].lock();
	}
}
