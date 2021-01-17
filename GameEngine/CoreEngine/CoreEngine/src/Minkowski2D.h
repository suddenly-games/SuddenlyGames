#pragma once

#include <vector>

#include "ColliderData.h"

namespace Engine
{
	namespace Physics
	{
		struct Line
		{
			Vector3 Point1, Point2, Normal;
		};

		struct ColliderInfo
		{
			ColliderData* Collider;
			int LastHitIndex;
			bool Clockwise;

			ColliderInfo(ColliderData& collider) : Collider(&collider), LastHitIndex(-1), Clockwise(false) {}
		};

		class MinkowskiDifference : public ColliderData
		{
		public:
			struct Side
			{
				bool IsFirstObject;
				int Side;
			};

			typedef std::vector<Side> SideVector;

			SideVector Sides;

			virtual void Calculate(const ColliderData& collider1, const ColliderData& collider2);
			bool Colliding();
			Vector3 MinimumTranslationVector();
		};

		class CircleMinkowskiDifference : public MinkowskiDifference
		{
		public:
			void Calculate(const ColliderData& collider, const Vector3& circleCenter, float radius);
			bool Colliding(int& circle);
			Vector3 MinimumTranslationVector(int circle);

		private:
			float CircleRadius;
		};

		typedef std::vector<ColliderData> ColliderVector;
		typedef std::vector<Line> LineVector;
		typedef std::vector<ColliderInfo> ColliderInfoVector;

		bool ObjectsAreColliding(const ColliderData& collider1, const ColliderData& collider2);

		VertexVector MinkowskiDifferenceBad(const ColliderData& collider1, const ColliderData& collider2);

		bool MeshContainsPoint(const VertexVector& mesh, const Vector3& point);

		Vector3 MinimumTranslationVector(const ColliderData& mesh, const Vector3& point);

		int GetEntrySide(const ColliderData& mesh, const Vector3& point, const Vector3& vector, float& distance);

		int GetEntrySideFromStart(const ColliderData& mesh, const Vector3& point, const Vector3& vector, float& distance, int startIndex);

		bool VectorApproachesCollider(const Vector3& point, const Vector3& vector, const Vector3& center, float radius);

		Vector3 GetVelocityOffset(const ColliderData& mesh, Vector3 velocity);

		void GetCollisionCandidates(const ColliderVector& differences, const Vector3& Velocity, LineVector& results);

		void GetClosestCollision(const ColliderInfoVector& colliders, Vector3 point, Vector3 velocity, ColliderInfoVector::const_iterator& ignore, ColliderInfoVector::const_iterator& closestCollider, float& closestDistance, int& closestSide);
		Vector3 GetVelocityOffset(const ColliderInfoVector& differences, const ColliderInfoVector::iterator& mesh, const Vector3& velocity, int entrySide, const Vector3& entryPoint, bool clockwise, const ColliderInfoVector::iterator& hitObject, int& hitSide);
		Vector3 GetVelocityOffset(const ColliderInfoVector& differences, Vector3 velocity);
		Vector3 GetCircleMinimumTranslationVector(const Vector3& difference, float radius);
	}
}