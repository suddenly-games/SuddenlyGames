#pragma once

#include "InputEvent.h"
#include "Object.h"

namespace Engine
{
	Declare_Enum(InputCode);
	Declare_Enum(InputType);
	Declare_Enum(InputState);

	class InputBase : public Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		virtual bool GetState() { return false; }
		virtual bool GetStateChanged() { return false; }
		virtual const Vector3& GetPosition() { return Nope; }
		virtual const Vector3& GetDelta() { return Nope; }
		virtual Enum::InputType GetType() { return Enum::InputType::None; }
		virtual Enum::InputCode GetCode() { return Enum::InputCode::None; }
		virtual std::string GetName() { return ""; }

		Event<std::shared_ptr<InputBase>> Began;
		Event<std::shared_ptr<InputBase>> Changed;
		Event<std::shared_ptr<InputBase>> Ended;

	private:
		static Vector3 Nope;

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputBase);
	};

	class InputData : public InputBase
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

		static std::shared_ptr<Object> CreateInput(InputObject* input);

		friend class UserInput;

	private:
		InputObject* Input = nullptr;

		static void RegisterEnums();

		Instantiable;

		Inherits_Class(InputBase);

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