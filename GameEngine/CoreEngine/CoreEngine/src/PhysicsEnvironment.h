#pragma once

#include "Object.h"
#include "AabbTree.h"
#include "PageAllocator.h"
#include "Minkowski2D.h"
#include "Ray.h"
#include "Collider2D.h"

namespace Engine
{
	class Terrain2DCollider;

	namespace Physics
	{
		class Collider2D;
		
		class RayCastResults
		{
		public:
			int GetCount() const;
			std::shared_ptr<Collider2D> GetHit(int index) const;
			float GetDistance(int index) const;
			Vector3 GetNormal(int index) const;
			void Push(const std::shared_ptr<Collider2D>& collider, float distance, const Vector3& normal);

		private:
			typedef std::vector<std::shared_ptr<Collider2D>> ColliderVector;
			typedef std::vector<float> FloatVector;
			typedef std::vector<Vector3> NormalVector;

			ColliderVector Colliders;
			FloatVector Distances;
			NormalVector Normals;

			Base_Class;

			Reflected_Type(RayCastResults);
		};
		
		class PhysicsEnvironment : public Object
		{
		public:
			~PhysicsEnvironment();

			void Initialize() {}
			void Update(float delta);

			void AddObject(const std::shared_ptr<Collider2D>& collider);
			void RemoveObject(const std::shared_ptr<Collider2D>& collider);
			void AddTerrain(const std::shared_ptr<Terrain2DCollider>& terrain);
			void RemoveTerrain(const std::shared_ptr<Terrain2DCollider>& terrain);
			RayCastResults CastRay(const Ray& ray);

		private:
			struct ColliderReference
			{
				int ID = -1;
				std::weak_ptr<Collider2D> Collider;
			};

			typedef std::vector<ColliderReference*> ReferenceVector;
			typedef std::vector<CollisionPair> CollisionPairVector;
			typedef std::vector<std::weak_ptr<Terrain2DCollider>> TerrainVector;

			ReferenceVector Colliders;
			TerrainVector Terrains;
			AabbTree ColliderPartitioning;
			PageAllocator<sizeof(ColliderReference)> ReferenceAllocator;
			MinkowskiDifference Difference;
			CircleMinkowskiDifference CircleDifference;
			CollisionPairVector CollisionBuffer;

			Instantiable;

			Inherits_Class(Object);

			Reflected(PhysicsEnvironment);
		};
	}

	using Physics::RayCastResults;

	//Class_Inherits(RayCastResults, CoreType);

	Define_Value_Type(RayCastResults);

	//Class_Inherits(Physics::PhysicsEnvironment, Object);
}