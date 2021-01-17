#include "LuaSource.h"

namespace Engine
{
	Reflect_Inherited(LuaSource, Object,
		Document_Class("");
		
		Bind_Function(GetPath,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetPath);
			);
		);
		
		Bind_Function(LoadSource,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, path);
				);

				Bind_Parameters_No_Return(LoadSource, path);
			);
		);
		
		Bind_Function(SaveSource,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, path);
				);

				Bind_Parameters_No_Return(SaveSource, path);
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
		);
		
		Bind_Function(GetSource,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);

				Overload_Parameters();

				Bind_Parameters(GetSource);
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
	);
}
