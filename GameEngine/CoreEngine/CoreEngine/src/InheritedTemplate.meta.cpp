#include "TypeTemplate.h"

Reflect_Type(InheritedTemplate,
	Document_Class(
		""
	);

	No_Reference; // Tells reflection that this should be passed by value.

	Bind_Function(ExampleMethod,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters();
			
			// Bind_Parameters_No_Return must be used for non-static function overloads with "void" return types.
			Bind_Parameters_No_Return(ExampleMethod);
		);

		Document("");
		// Static tag is used to mark a static function overload
		Static Function_Overload
		(
			Document("");
			Overload_Returns(int);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(int, arg1);
			);
			
			/*
				Static_Bind_Parameters must be used for static overloads with a non-void return type.
				Static_Bind_Parameters_No_Return must be used for static overloads with a void return type.
			*/
			Static_Bind_Parameters(ExampleMethod, arg1);
		);
	);
);
