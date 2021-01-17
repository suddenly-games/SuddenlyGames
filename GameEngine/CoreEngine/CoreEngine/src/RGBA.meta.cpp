#include "RGBA.h"

Reflect_Type(RGBA,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(float, R);
	
	Document("");
	Read_Only Archivable Class_Member(float, G);

	Document("");
	Read_Only Archivable Class_Member(float, B);

	Document("");
	Read_Only Archivable Class_Member(float, A);

	Bind_Function(RGBA,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters();

			Bind_Constructor();
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, r);
	
				Document("");
				Function_Parameter(float, g);
	
				Document("");
				Function_Parameter(float, b);
	
				Document("");
				Function_Parameter_Default(float, a, 1);
			);

			Bind_Constructor(r, g, b, a);
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
		
			Overload_Parameters
			(
				Document("");
				Function_Parameter(unsigned int, colorHex);
			);

			Bind_Constructor(colorHex);
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, vector);
			);

			Bind_Constructor(vector);
		);
	);
	
	Bind_Function(Lerp,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(RGBA);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(RGBA, end);
	
				Document("");
				Function_Parameter(float, t);
			);
			
			Bind_Parameters(Lerp, end, t);
		);
	);
	
	//Bind_Function(ARGB,
	//
	//	Document("");
	//	Function_Overload
	//	(
	//		Document("");
	//		Overload_Returns(unsigned int);
	//
	//		Overload_Parameters();
	//		
	//		Bind_Parameters(ARGB);
	//	);
	//);
	//
	//Bind_Function(ABGR,
	//
	//	Document("");
	//	Function_Overload
	//	(
	//		Document("");
	//		Overload_Returns(unsigned int);
	//
	//		Overload_Parameters();
	//		
	//		Bind_Parameters(ABGR);
	//	);
	//);
	
	Bind_Function(operator==,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(RGBA, other);
			);
			
			Bind_Parameters(operator==, other);
		);
	);
	
	Bind_Function(operator!=,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(RGBA, other);
			);
			
			Bind_Parameters(operator!=, other);
		);
	);
	
	//Bind_Function(Hex,
	//
	//	Document("");
	//	Function_Overload
	//	(
	//		Document("");
	//		Overload_Returns(unsigned int);
	//
	//		Overload_Parameters();
	//		
	//		Bind_Parameters(operator unsigned int);
	//	);
	//);
	
	Bind_Function(Vector,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);
	
			Overload_Parameters();
			
			Bind_Parameters(operator Vector3);
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
