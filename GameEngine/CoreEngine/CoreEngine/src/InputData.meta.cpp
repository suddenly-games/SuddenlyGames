#include "LuaInput.h"

namespace Engine
{
	Reflect_Inherited(InputData, Object,
		Document_Class("");

		Bind_Function(GetState,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(GetState);
			);
		);

		Bind_Function(GetStateChanged,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(GetStateChanged);
			);
		);

		Bind_Function(GetPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetPosition);
			);
		);

		Bind_Function(GetDelta,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetDelta);
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

		Bind_Function(GetName,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetName);
			);
		);
	);
}