#pragma once

#include "InputEvent.h"
#include "Object.h"

namespace Engine
{
	Declare_Enum(InputCode);
	Declare_Enum(InputType);
	Declare_Enum(InputState);

	class InputData : public Object
	{
	public:
		void Initialize();
		void Update(float) {}

		void Configure(InputObject* input);

		bool GetState();
		bool GetStateChanged();
		const Vector3& GetPosition();
		const Vector3& GetDelta();
		Enum::InputType GetType();
		Enum::InputCode GetCode();
		std::string GetName();

		Event<std::shared_ptr<InputData>> Began;
		Event<std::shared_ptr<InputData>> Changed;
		Event<std::shared_ptr<InputData>> Ended;

		static std::shared_ptr<Object> CreateInput(InputObject* input);

		friend class UserInput;

	private:
		InputObject* Input = nullptr;

		static void RegisterEnums();

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputData);
	};

	//Class_Inherits(InputData, Object);

	class UserInput : public Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		void Configure(InputHandler* handler);

		bool GetState(Enum::InputCode code);
		bool GetStateChanged(Enum::InputCode code);
		const Vector3& GetPosition(Enum::InputCode code);
		const Vector3& GetDelta(Enum::InputCode code);
		Enum::InputType GetType(Enum::InputCode code);
		const char* GetName(Enum::InputCode code);
		std::shared_ptr<InputData> GetInput(Enum::InputCode code);

	private:
		typedef std::vector<std::weak_ptr<InputData>> InputVector;

		InputVector Inputs;
		InputHandler* Handler = nullptr;

		Instantiable;

		Inherits_Class(Object);

		Reflected(UserInput);
	};

	//Class_Inherits(UserInput, Object);
}