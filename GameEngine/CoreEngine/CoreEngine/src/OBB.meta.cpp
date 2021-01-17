#include "OBB.h"

#include "ModelAsset.h"

Reflect_Type(OBB,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Size);
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Center);

	Document("");
	Read_Only Archivable Class_Member(Vector3, Transformation);

	Bind_Function(OBB,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters();

			Bind_Constructor();
		);
	);
	
	Bind_Function(Compute,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(OBB);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
			);
			
			Bind_Parameters(Compute, model);
		);
	);
);
