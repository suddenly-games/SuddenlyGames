#include "ColliderData.h"

#include <cstdarg>
#include <limits>
#include <algorithm>

namespace Engine
{
	namespace Physics
	{
		ColliderData::ColliderData() { }

		ColliderData::ColliderData(const ColliderData& other, const Matrix3& transformation)
		{
			TransformFrom(other, transformation);

			RecalculateLimitVertices();
			RecalculateNormals();
			RecalculateCenter();
			RecalculateBoundingRadius();
		}

		void ColliderData::TransformFrom(const ColliderData& other, const Matrix3& transformation)
		{
			Vertices.clear();

			for (VertexVector::const_iterator i = other.Vertices.begin(); i != other.Vertices.end(); ++i)
				Vertices.push_back(transformation * *i);

			RecalculateLimitVertices();
			RecalculateNormals();
			RecalculateCenter();
			RecalculateBoundingRadius();
		}

		void ColliderData::Translate(const Vector3& translation)
		{
			Center += translation;

			for (VertexVector::iterator i = Vertices.begin(); i != Vertices.end(); ++i)
				*i += translation;
		}

		void ColliderData::RecalculateLimitVertices()
		{
			if (Vertices.size() > 0)
			{
				BoundingBox.Min = Vertices[0];
				BoundingBox.Max = Vertices[0];
			}
			else
			{
				BoundingBox.Min.Set(0, 0);
				BoundingBox.Max.Set(0, 0);
			}

			float highestPoint = -std::numeric_limits<float>::max();
			float lowestPoint = std::numeric_limits<float>::max();

			TopVertex = 0;
			BottomVertex = 0;

			for (int i = 0; i < (int)Vertices.size(); ++i)
			{
				float y = Vertices[i].Y;

				if (y > highestPoint)
				{
					highestPoint = y;
					TopVertex = i;
				}

				if (y < lowestPoint)
				{
					lowestPoint = y;
					BottomVertex = i;
				}

				BoundingBox.Expand(Vertices[i]);
			}

			BoundingBox.Min.Z = -1;
			BoundingBox.Max.Z = 1;
		}

		void ColliderData::RecalculateNormals()
		{
			Normals.clear();

			int size = Vertices.size();

			for (int i = 0; i < size; ++i)
			{
				Vector3 difference = Vertices[(i + 1) % size] - Vertices[i];

				Normals.push_back(Vector3(-difference.Y, difference.X).Normalize());
			}
		}

		void ColliderData::RecalculateCenter()
		{
			Center.Set(0, 0);

			int size = Vertices.size();

			for (VertexVector::iterator i = Vertices.begin(); i != Vertices.end(); ++i)
				Center += 1.0f / size * *i;
		}

		void ColliderData::RecalculateBoundingRadius()
		{
			RadiusSquared = 0;

			for (VertexVector::iterator i = Vertices.begin(); i != Vertices.end(); ++i)
			{
				float distance = (Center - *i).SquareLength();

				if (distance > RadiusSquared)
					RadiusSquared = distance;
			}
		}

		void ColliderData::Clear()
		{
			Vertices.clear();
			Normals.clear();
		}

		void ColliderData::Push(Vector3 vertex)
		{
			vertex.W = 1;

			Vertices.push_back(vertex);
		}

		void ColliderData::Finalize()
		{
			RecalculateLimitVertices();
			RecalculateNormals();
			RecalculateCenter();
			RecalculateBoundingRadius();
		}
	}
}