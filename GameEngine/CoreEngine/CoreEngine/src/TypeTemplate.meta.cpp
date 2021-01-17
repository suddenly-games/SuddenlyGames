#include "TypeTemplate.h"

Reflect_Type(TemplateType,
	Document_Class(
		""
	);

	No_Reference; // Tells reflection that this should be passed by value.
	
	Document("");
	/*
		Read_Only tag prevents Lua from writing to a member, but still allows reading.
		Archivable tag marks member for file I/O
		Static tag marks member as being static. In Lua, these members can be accessed through "TypeName.StaticMemberName".
	*/
	Read_Only Archivable Class_Member(int, ExampleMember);

	Bind_Function(TemplateType,
		// Multiple overload definitions can be placed in here
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing; // Required for constructors and functions with "void" return types.
	
			/*
				Definition region for function overload parameters.
				Creates functors that unpack values off of Lua's stack to pass to the function internally.
			*/
			Overload_Parameters
			(
				Document("");
				Function_Parameter(int, exampleParam);

				Document("");
				Function_Parameter_Default(int, exampleParam2, 0);
			);

			// Bind_Constructor must be used to bind constructor overloads.
			Bind_Constructor(exampleParam, exampleParam2);
		);
	);
	
	/*
		The function name used here doesn't need to match the bound functions in the specified overloads.
		Lua will use the name specified here.
	*/
	Bind_Function(operator string,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(std::string);
	
			Overload_Parameters();
			
			// Real function name must be referenced here
			Bind_Parameters(operator std::string);
		);
	);

	// Many operator overloads will be used internally by Lua if the names match correctly in reflection.
	Bind_Function(operator-,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(int);
	
			Overload_Parameters();
			
			// Bind_Parameters must be used for non-static function overloads with a non-void return type.
			Bind_Parameters(operator-);
		);
	);
);
