#pragma once

#include "Object.h"

#include "Aabb.h"
#include "ColliderData.h"

namespace Engine
{
	namespace Physics
	{
		struct CollisionPair;
		class CollisionGroup;
		class ColliderAsset;

		class Collision
		{
		public:
			std::weak_ptr<Object> OtherCollider;
			Vector3 ContactPoint;
			Vector3 Displacement;

			Base_Class;

			Reflected_Type(Collision);
		};

		class Collider2D : public Object
		{
		public:
			bool IsCircle = false;
			std::weak_ptr<CollisionGroup> Group;
			std::weak_ptr<ColliderAsset> Asset;
			ColliderData Data;

			~Collider2D() {}

			void Initialize();
			void Update(float) {}

			void UpdateData();
			Aabb GetBoundingBox() const;
			bool HasMoved() const;
			float GetRadius() const;
			Vector3 GetCenter() const;
			float GetWidth(const Vector3& axis) const;
			bool CanCollideWith(const std::shared_ptr<Object>& otherCollider) const;
			void Collided(const CollisionPair& collisionPair);
			int GetCollisions() const;
			Collision GetCollision(int index) const;

		private:

			typedef std::vector<Collision> CollisionVector;

			bool Moved = false;
			float Radius = 0;
			Aabb BoundingBox;
			Vector3 Center;
			CollisionVector Collisions;

			Instantiable;

			Inherits_Class(Object);

			Reflected(Collider2D);
		};
	}

	using Physics::Collision;

	//Class_Inherits(Collision, CoreType);

	Define_Value_Type(Collision);

	//Class_Inherits(Physics::Collider2D, Object);

	namespace Physics
	{
		struct CollisionPair
		{
			std::shared_ptr<Collider2D> Collider1;
			std::shared_ptr<Collider2D> Collider2;

			Vector3 ContactPoint;
			Vector3 Displacement;
		};
	}
}