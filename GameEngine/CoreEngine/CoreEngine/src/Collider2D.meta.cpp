#include "Collider2D.h"

#include "ColliderAsset.h"
#include "CollisionGroup.h"

namespace Engine
{
	Reflect_Type(Collision,
		Document_Class("");

		No_Reference;

		Document("");
		Read_Only Archivable Class_Member(std::weak_ptr<Object>, OtherCollider);

		Document("");
		Read_Only Archivable Class_Member(Vector3, ContactPoint);

		Document("");
		Read_Only Archivable Class_Member(Vector3, Displacement);
	);
	
	using Physics::Collider2D;

	Reflect_Inherited(Collider2D, Object,
		Document_Class("");

		Document("");
		Read_Only Archivable Class_Member(bool, IsCircle);

		Document("");
		Read_Only Archivable Class_Member(std::weak_ptr<CollisionGroup>, Group);

		Document("");
		Read_Only Archivable Class_Member(std::weak_ptr<ColliderAsset>, Asset);
		
		Bind_Function(GetBoundingBox,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Aabb);
	
				Overload_Parameters();

				Bind_Parameters(GetBoundingBox);
			);
		);
		
		Bind_Function(HasMoved,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
	
				Overload_Parameters();

				Bind_Parameters(HasMoved);
			);
		);
		
		Bind_Function(GetRadius,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
	
				Overload_Parameters();

				Bind_Parameters(GetRadius);
			);
		);
		
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
		
		Bind_Function(GetWidth,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(Vector3, axis);
				);

				Bind_Parameters(GetWidth, axis);
			);
		);
		
		Bind_Function(CanCollideWith,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Object>, otherCollider);
				);

				Bind_Parameters(CanCollideWith, otherCollider);
			);
		);
		
		Bind_Function(GetCollisions,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
	
				Overload_Parameters();

				Bind_Parameters(GetCollisions);
			);
		);
		
		Bind_Function(GetCollision,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Collision);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(int, index);
				);

				Bind_Parameters(GetCollision, index);
			);
		);
	);
}