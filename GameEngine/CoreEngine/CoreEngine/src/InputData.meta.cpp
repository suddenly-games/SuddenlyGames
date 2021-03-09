#include "LuaInput.h"

namespace Engine
{
	Reflect_Inherited(InputObject, Object,
		Document_Class("");

		Bind_Function(GetState,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::BoundDevice>, device, Enum::BoundDevice::Any);
				);

				Bind_Parameters(GetState, device);
			);
		);

		Bind_Function(GetStateChanged,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::BoundDevice>, device, Enum::BoundDevice::Any);
				);

				Bind_Parameters(GetStateChanged, device);
			);
		);

		Bind_Function(GetStateEnum,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::InputState>);

				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::BoundDevice>, device, Enum::BoundDevice::Any);
				);

				Bind_Parameters(GetStateEnum, device);
			);
		);

		Bind_Function(GetPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::BoundDevice>, device, Enum::BoundDevice::Any);
				);

				Bind_Parameters(GetPosition, device);
			);
		);

		Bind_Function(GetDelta,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(LuaEnum<Enum::BoundDevice>, device, Enum::BoundDevice::Any);
				);

				Bind_Parameters(GetDelta, device);
			);
		);

		Bind_Function(GetType,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::InputType>);

				Overload_Parameters();

				Bind_Parameters(GetType);
			);
		);

		Bind_Function(GetCode,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::InputCode>);

				Overload_Parameters();

				Bind_Parameters(GetCode);
			);
		);

		Bind_Function(GetDevice,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::BoundDevice>);

				Overload_Parameters();

				Bind_Parameters(GetDevice);
			);
		);
	);

	Reflect_Inherited(InputDevice, InputObject,
		Document_Class("");
	);

	Reflect_Inherited(InputBinding, InputObject,
		Document_Class("");

		Document("");
		Class_Member(LuaEnum<Enum::InputMode>, BindingMode);
		
		Bind_Function(Bind,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<InputObject>, input);
				);

				Bind_Parameters_No_Return(Bind, input);
			);
		);
		
		Bind_Function(Unbind,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<InputObject>, input);
				);

				Bind_Parameters_No_Return(Unbind, input);
			);
		);

		Bind_Function(GetBindings,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetBindings);
			);
		);

		Bind_Function(GetBinding,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<InputObject>);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetBinding, index);
			);
		);

		Bind_Function(IsBound,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<InputObject>, input);
				);

				Bind_Parameters(IsBound, input);
			);
		);
	);

	Enum_Definition(InputMode,
		Document_Enum("");
		
		Enum_Item(Or);
		Enum_Item(And);
		Enum_Item(Xor);
	);
}	