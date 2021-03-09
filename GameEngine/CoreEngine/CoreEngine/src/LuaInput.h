#pragma once

#include "InputEvent.h"
#include "Object.h"

namespace Engine
{
	Declare_Enum(InputCode);
	Declare_Enum(InputType);
	Declare_Enum(InputState);
	Declare_Enum(BoundDevice);

	class InputData
	{
	public:
		bool GetState() const { return State; }
		Enum::InputState GetStateEnum() const { return StateEnum; }
		bool GetStateChanged() const { return StateChanged; }
		const Vector3& GetPosition() const { return Position; }
		const Vector3& GetDelta() const { return Delta; }

		bool SetState(bool state, bool suppressChanged = false);
		bool SetPosition(const Vector3& position, bool suppressChanged = false);

	private:
		bool State = false;
		bool StateChanged = false;
		Enum::InputState StateEnum = Enum::InputState::Idle;
		Vector3 Position;
		Vector3 Delta;
	};

	class InputObject : public Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		bool GetState(Enum::BoundDevice device = Enum::BoundDevice::Any) const {
			return Inputs[device].GetState();
		}
		bool GetStateChanged(Enum::BoundDevice device = Enum::BoundDevice::Any) const { return Inputs[device].GetStateChanged(); }
		Enum::InputState GetStateEnum(Enum::BoundDevice device = Enum::BoundDevice::Any) const { return Inputs[device].GetStateEnum(); }
		const Vector3& GetPosition(Enum::BoundDevice device = Enum::BoundDevice::Any) const { return Inputs[device].GetPosition(); }
		const Vector3& GetDelta(Enum::BoundDevice device = Enum::BoundDevice::Any) const { return Inputs[device].GetDelta(); }

		virtual Enum::InputType GetType() const { return Enum::InputType::None; }
		virtual Enum::InputCode GetCode() const { return Enum::InputCode::None; }
		virtual Enum::BoundDevice GetDevice() const { return Enum::BoundDevice::Any; }

		Event<const std::shared_ptr<InputObject>&> Began;
		Event<const std::shared_ptr<InputObject>&> Changed;
		Event<const std::shared_ptr<InputObject>&> Ended;
		
	protected:
		void SetState(bool state, Enum::BoundDevice device = Enum::BoundDevice::Any, bool suppressChanged = false, const std::shared_ptr<InputObject>& input = nullptr);
		void SetPosition(const Vector3& position, Enum::BoundDevice device = Enum::BoundDevice::Any, bool suppressChanged = false, const std::shared_ptr<InputObject>& input = nullptr);

	private:
		static Vector3 Nope;

		InputData Inputs[Enum::BoundDevice::Count] = {};

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputObject);
	};

	class InputDevice : public InputObject
	{
	public:
		void Initialize() {}
		void Update(float) {}

		void Configure(::InputObject* input);

		Enum::InputType GetType();
		Enum::InputCode GetCode();
		Enum::BoundDevice GetDevice();

		static std::shared_ptr<Object> CreateInput(::InputObject* input);

		friend class UserInput;

	private:
		::InputObject* Input = nullptr;

		Instantiable;

		Inherits_Class(InputObject);

		Reflected(InputDevice);
	};
	
	class InputBinding : public InputObject
	{
	public:
		struct InputModeEnum
		{
			enum InputMode
			{
				Or,
				And,
				Xor,

				Count
			};
		};

		typedef InputModeEnum::InputMode InputMode;

		~InputBinding();

		InputMode BindingMode = InputMode::Or;

		void Initialize() {}
		void Update(float);

		Enum::InputType GetType() const { return Enum::InputType::Custom; }
		Enum::InputCode GetCode() const { return Enum::InputCode::Custom; }

		void Bind(const std::shared_ptr<InputObject>& input);
		void Unbind(const std::shared_ptr<InputObject>& input);
		int GetBindings() const;
		std::shared_ptr<InputObject> GetBinding(int index) const;
		bool IsBound(const std::shared_ptr<InputObject>& input) const;

	private:
		struct BindingData
		{
			bool State = false;
			int BeganConnection = -1;
			int ChangedConnection = -1;
			int EndedConnection = -1;
			Enum::BoundDevice Device = Enum::BoundDevice::Any;
			std::weak_ptr<InputObject> Input;
		};

		typedef std::vector<BindingData> BindingVector;

		BindingVector Bindings;

		void SetState(bool state, int index, bool suppressUpdates = false);
		void UpdateState(Enum::BoundDevice device, bool suppressUpdates = false);
		void SetPosition(const Vector3& position, int index, bool suppressUpdates = false);
		void Unbind(int index);
		int Find(const std::shared_ptr<InputObject>& input) const;

		Instantiable;

		Inherits_Class(InputObject);

		Reflected(InputBinding);
	};

	class UserInput : public Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		void Configure(InputHandler* handler);

		bool GetState(Enum::InputCode code);
		bool GetStateChanged(Enum::InputCode code);
		Enum::InputState GetStateEnum(Enum::InputCode code);
		const Vector3& GetPosition(Enum::InputCode code);
		const Vector3& GetDelta(Enum::InputCode code);
		Enum::InputType GetType(Enum::InputCode code);
		const char* GetName(Enum::InputCode code);
		std::shared_ptr<InputDevice> GetInput(Enum::InputCode code);

		std::shared_ptr<InputObject> InputBegan;
		std::shared_ptr<InputObject> InputChanged;
		std::shared_ptr<InputObject> InputEnded;

	private:
		typedef std::vector<std::weak_ptr<InputDevice>> InputVector;

		InputVector Inputs;
		InputHandler* Handler = nullptr;

		Instantiable;

		Inherits_Class(Object);

		Reflected(UserInput);
	};
}

namespace Enum
{
	typedef Engine::InputBinding::InputMode InputMode;
}

namespace Engine
{
	Declare_Enum(InputMode);
}