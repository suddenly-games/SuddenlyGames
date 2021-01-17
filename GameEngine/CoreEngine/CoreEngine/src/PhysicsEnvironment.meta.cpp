#include "PhysicsEnvironment.h"

#include "Collider2D.h"

namespace Engine
{
	Reflect_Type(RayCastResults,
		Document_Class("");

		No_Reference;
		
		Bind_Function(GetCount,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
	
				Overload_Parameters();

				Bind_Parameters(GetCount);
			);
		);
		
		Bind_Function(GetHit,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Collider2D>);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetHit, index);
			);
		);
		
		Bind_Function(GetDistance,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetDistance, index);
			);
		);
		
		Bind_Function(GetNormal,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetNormal, index);
			);
		);
	);

	using Physics::PhysicsEnvironment;

	Reflect_Inherited(PhysicsEnvironment, Object,
		Document_Class("");
		
		Bind_Function(AddObject,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Collider2D>, collider);
				);

				Bind_Parameters_No_Return(AddObject, collider);
			);
		);
		
		Bind_Function(RemoveObject,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Collider2D>, collider);
				);

				Bind_Parameters_No_Return(RemoveObject, collider);
			);
		);
		
		Bind_Function(CastRay,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(RayCastResults);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(Ray, ray);
				);

				Bind_Parameters(CastRay, ray);
			);
		);
	);
}