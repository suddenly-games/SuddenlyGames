#include "BoundingSphere.h"

#include "ModelAsset.h"

Reflect_Type(BoundingSphere,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(float, Radius);
	
	Document("");
	Read_Only Archivable Class_Member(Vector3, Center);

	Bind_Function(BoundingSphere,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters(
				Document("");
				Function_Parameter_Default(float, radius, 0);

				Document("");
				Function_Parameter_Default(Vector3, center, Vector3());
			);

			Bind_Constructor(radius, center);
		);
	);
	
	Bind_Function(ComputeCentroid,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(BoundingSphere);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
			);
			
			Bind_Parameters(ComputeCentroid, model);
		);
	);
	
	Bind_Function(ComputeRitter,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(BoundingSphere);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
			);
			
			Bind_Parameters(ComputeRitter, model);
		);
	);
	
	Bind_Function(ComputeLarson,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(BoundingSphere);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
			);
			
			Bind_Parameters(ComputeLarson, model);
		);
	);
	
	Bind_Function(ComputePCA,
	
		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(BoundingSphere);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(std::shared_ptr<Engine::ModelAsset>, model);
			);
			
			Bind_Parameters(ComputePCA, model);
		);
	);
);
