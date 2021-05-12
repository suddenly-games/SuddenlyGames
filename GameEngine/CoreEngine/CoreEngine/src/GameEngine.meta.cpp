#include "GameEngine.h"

namespace Engine
{
	Reflect_Type(ProcessArguments,
		Document_Class("");

		No_Reference;

		Bind_Function(GetArguments,
			Document("");
			Function_Overload
			(
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetArguments);
			);
		);

		Bind_Function(GetArgument,
			Document("");
			Function_Overload
			(
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetArgument, index);
			);
		);

		Bind_Function(HasArgumentFlag,
			Document("");
			Function_Overload
			(
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters(HasArgumentFlag, name);
			);
		);

		Bind_Function(GetArgumentValue,
			Document("");
			Function_Overload
			(
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters(GetArgumentValue, name);
			);
		);

		Bind_Function(PushArgument,
			Document("");
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, argument);
				);

				Bind_Parameters_No_Return(PushArgument, argument);
			);
		);

		Bind_Function(InsertArgumentFlag,
			Document("");
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters_No_Return(InsertArgumentFlag, name);
			);
		);

		Bind_Function(InsertArgument,
			Document("");
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);

					Document("");
					Function_Parameter(std::string, value);
				);

				Bind_Parameters_No_Return(InsertArgument, name, value);
			);
		);
	);

	Reflect_Inherited(GameEngine, Object,
		Document_Class("");

		Bind_Function(GetArguments,
			Document("");
			Function_Overload
			(
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetArguments);
			);
		);

		Bind_Function(GetArgument,
			Document("");
			Function_Overload
			(
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetArgument, index);
			);
		);

		Bind_Function(HasArgumentFlag,
			Document("");
			Function_Overload
			(
				Overload_Returns(bool);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters(HasArgumentFlag, name);
			);
		);

		Bind_Function(GetArgumentValue,
			Document("");
			Function_Overload
			(
				Overload_Returns(std::string);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, name);
				);

				Bind_Parameters(GetArgumentValue, name);
			);
		);

		Bind_Function(SpawnProcess,
			Document("");
			Function_Overload
			(
				Overload_Returns(int);
		
				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, path);
					
					Document("");
					Function_Parameter_Default(ProcessArguments, processArguments, ProcessArguments());
				);
		
				Bind_Parameters(SpawnProcess, path, processArguments);
			);
		);
	);
}