#include "LuaScript.h"

#include "LuaSource.h"

namespace Engine
{
	using Enum::ScriptStatus;

	Enum_Definition(ScriptStatus,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(Idle);
		
		Document_Item("");
		Enum_Item(Running);
		
		Document_Item("");
		Enum_Item(Yielded);
		
		Document_Item("");
		Enum_Item(Dead);
	);

	Reflect_Inherited(LuaScript, Object,
		Document_Class("");
		
		Bind_Function(GetSource,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<LuaSource>);

				Overload_Parameters();

				Bind_Parameters(GetSource);
			);
		);
		
		Bind_Function(GetSourceCode,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetSourceCode);
			);
		);
		
		Bind_Function(GetVersion,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetVersion);
			);
		);
		
		Bind_Function(UpToDate,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(UpToDate);
			);
		);
		
		Bind_Function(AutoRunEnabled,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(AutoRunEnabled);
			);
		);
		
		Bind_Function(GetStatus,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::ScriptStatus>);

				Overload_Parameters();

				Bind_Parameters(GetStatus);
			);
		);
		
		Bind_Function(SetSource,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, source);
				);

				Bind_Parameters_No_Return(SetSource, source);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<LuaSource>, source);
				);

				Bind_Parameters_No_Return(SetSource, source);
			);
		);
		
		Bind_Function(SetAutoRun,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(bool, mode);
				);

				Bind_Parameters_No_Return(SetAutoRun, mode);
			);
		);
		
		Bind_Function(Run,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters();

				Bind_Parameters_No_Return(Run);
			);
		);
		
		Bind_Function(Stop,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters();

				Bind_Parameters_No_Return(Stop);
			);
		);
		
		Bind_Function(Reload,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters();

				Bind_Parameters_No_Return(Reload);
			);
		);
		
		Bind_Function(GetData,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters();

				Bind_Lua_Function(GetData);
			);
		);
	);
}
