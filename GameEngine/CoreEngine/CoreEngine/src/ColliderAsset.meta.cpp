#include "ColliderAsset.h"

namespace Engine
{
	using Physics::ColliderAsset;

	Reflect_Inherited(ColliderAsset, Object,
		Document_Class("");
		
		Bind_Function(ConfigureTriangleMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureTriangleMesh);
			);
		);
		
		Bind_Function(ConfigureSquareMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureSquareMesh);
			);
		);
		
		Bind_Function(ConfigureOctagonMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureOctagonMesh);
			);
		);
		
		Bind_Function(ConfigureShipMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureShipMesh);
			);
		);
		
		Bind_Function(ConfigureChargerShieldMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureChargerShieldMesh);
			);
		);
		
		Bind_Function(ConfigureWormHeadMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureWormHeadMesh);
			);
		);
		
		Bind_Function(ConfigureWormTailMesh,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters();

				Bind_Parameters_No_Return(ConfigureWormTailMesh);
			);
		);
	);
}