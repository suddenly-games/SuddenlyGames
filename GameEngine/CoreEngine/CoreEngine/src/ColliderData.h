#pragma once

#include <vector>
#include "Matrix3.h"
#include "Aabb.h"

namespace Engine
{
	namespace Physics
	{
		typedef std::vector<Vector3> VertexVector;

		class ColliderData
		{
		public:
			VertexVector Vertices;
			VertexVector Normals;
			Vector3 Center;
			float RadiusSquared = 0;
			int TopVertex = -1;
			int BottomVertex = -1;
			Aabb BoundingBox;

			ColliderData();
			ColliderData(const ColliderData& other, const Matrix3& transformation = Matrix3());

			void TransformFrom(const ColliderData& other, const Matrix3& transformation);
			void Translate(const Vector3& translation);
			void RecalculateLimitVertices();
			void RecalculateNormals();
			void RecalculateCenter();
			void RecalculateBoundingRadius();
			void Clear();
			void Push(Vector3 vertex);
			void Finalize();
		};
	}
}