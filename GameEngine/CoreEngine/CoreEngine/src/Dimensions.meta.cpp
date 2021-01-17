#include "Dimensions.h"

Reflect_Type(Dimensions,
	Document_Class("");
		
	Document("");
	Archivable Class_Member(unsigned int, Width);

	Document("");
	Archivable Class_Member(unsigned int, Height);

	Bind_Function(Dimensions,
		
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
			
			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(unsigned int, width, 0);
					
				Document("");
				Function_Parameter_Default(unsigned int, height, 0);
			);
			
			Bind_Constructor(width, height);
		);
	);

	Bind_Function(operator string,
		
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(std::string);
			
			Overload_Parameters();
			
			Bind_Parameters(operator std::string);
		);
	);
);