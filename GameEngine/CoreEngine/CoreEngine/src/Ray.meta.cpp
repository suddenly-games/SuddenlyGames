#include "Ray.h"

Reflect_Type(Ray,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Start);
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Direction);

	Bind_Function(Ray,
	
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
				Function_Parameter(Vector3, start);

				Document("");
				Function_Parameter(Vector3, direction);
			);

			Bind_Constructor(start, direction);
		);
	);
);
