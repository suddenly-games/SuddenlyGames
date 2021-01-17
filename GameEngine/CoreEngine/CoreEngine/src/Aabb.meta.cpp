#include "Aabb.h"

#include "ModelAsset.h"

Reflect_Type(Aabb,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Min);
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Max);

	Bind_Function(Aabb,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters(
				Document("");
				Function_Parameter_Default(Vector3, min, Vector3());

				Document("");
				Function_Parameter_Default(Vector3, max, Vector3());
			);

			Bind_Constructor(min, max);
		);
	);
	
	//Bind_Function(Compute,
	//
	//	Document("");
	//	Static Function_Overload
	//	(
	//		Document("");
	//		Overload_Returns(Aabb);
	//
	//		Overload_Parameters
	//		(
	//			Document("");
	//			Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
	//		);
	//		
	//		Bind_Parameters(Compute, model);
	//	);
	//);

	Bind_Function(GetCenter,
		
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);
			
			Overload_Parameters();
			
			Bind_Parameters(GetCenter);
		);
	);

	Bind_Function(GetSize,
		
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);
			
			Overload_Parameters();
			
			Bind_Parameters(GetSize);
		);
	);
);
