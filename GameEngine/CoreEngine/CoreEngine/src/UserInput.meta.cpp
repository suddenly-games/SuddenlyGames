#include "LuaInput.h"

namespace Engine
{
	Reflect_Inherited(UserInput, Object,
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
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetState, code);
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
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetStateChanged, code);
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
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetPosition, code);
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
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetDelta, code);
			);
		);

		Bind_Function(GetType,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::InputType>);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetType, code);
			);
		);

		Bind_Function(GetName,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetName, code);
			);
		);

		Bind_Function(GetInput,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<InputDevice>);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(LuaEnum<Enum::InputCode>, code);
				);

				Bind_Parameters(GetInput, code);
			);
		);
	);
}