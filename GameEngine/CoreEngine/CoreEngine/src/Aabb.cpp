#include "Aabb.h"

#include <algorithm>

#include "MeshLoader.h"

// Helper functions
namespace {
  Vector3 MinVector(const Vector3& lhs, const Vector3& rhs)
  {
    Vector3 minVector;

    for (unsigned i = 0; i < 3; ++i)
    {
      minVector[i] = (lhs[i] < rhs[i]) ? lhs[i] : rhs[i];
    }

    return minVector;
  }

  Vector3 MaxVector(const Vector3& lhs, const Vector3& rhs)
  {
    Vector3 maxVector;

    for (unsigned i = 0; i < 3; ++i)
    {
      maxVector[i] = (lhs[i] > rhs[i]) ? lhs[i] : rhs[i];
    }

    return maxVector;
  }
}

Aabb::Aabb(const Vector3& min, const Vector3& max) : Min(min), Max(max)
{
	Min.W = 1;
	Max.W = 1;
}

void Aabb::Expand(const Vector3& point)
{
	Min.X = std::min(Min.X, point.X);
	Min.Y = std::min(Min.Y, point.Y);
	Min.Z = std::min(Min.Z, point.Z);
	Max.X = std::max(Max.X, point.X);
	Max.Y = std::max(Max.Y, point.Y);
	Max.Z = std::max(Max.Z, point.Z);
}

bool Aabb::ContainsPoint(const Vector3& point) const
{
	return (
		InRange(point.X, Min.X, Max.X) &&
		InRange(point.Y, Min.Y, Max.Y) &&
		InRange(point.Z, Min.Z, Max.Z)
	);
}

bool Aabb::Intersects(const Aabb& box) const
{
	return Aabb(Min, Max + box.GetSize()).ContainsPoint(box.Max);
}

bool Aabb::InRange(float x, float min, float max) const
{
	return x >= min && x <= max;
}

Vector3 Aabb::GetCenter() const
{
	return 0.5f * (Min + Max);
}

Vector3 Aabb::GetSize() const
{
	return Max - Min;
}

typename Enum::IntersectionType Aabb::Intersects(const Plane& plane, float epsilon) const
{
	Vector3 center = 0.5f * (Min + Max);
	Vector3 halfExtent = Max - center;

	Vector3 normal = plane;
	Vector3 absNormal;

	absNormal.X = std::abs(normal.X);
	absNormal.Y = std::abs(normal.Y);
	absNormal.Z = std::abs(normal.Z);
	
	normal.W = 0;

	float projectionRadius = halfExtent * absNormal;
	float centerProjectionRadius = normal * center - plane.W;

	if (std::abs(centerProjectionRadius) <= projectionRadius)
			return Enum::IntersectionType::Overlaps;
	else if (centerProjectionRadius > 0)
		return Enum::IntersectionType::Inside;
	else
		return Enum::IntersectionType::Outside;
	
	/*Vector3 halfExtent = 0.5f * (Max - Min);
	Vector3 center = 0.5f * (Min + Max);

	if (plane.X < 0)
		halfExtent.X *= -1;

	if (plane.Y < 0)
		halfExtent.Y *= -1;

	if (plane.Z < 0)
		halfExtent.Z *= -1;

	Enum::IntersectionType extent1 = plane.Compare(center + halfExtent, epsilon);
	Enum::IntersectionType extent2 = plane.Compare(center - halfExtent, epsilon);

	if (extent1 == Enum::IntersectionType::Coplanar)
		extent1 = Enum::IntersectionType::Overlaps;

	if (extent2 == Enum::IntersectionType::Coplanar)
		extent2 = Enum::IntersectionType::Overlaps;

	if (extent1 == extent2 || extent2 == Enum::IntersectionType::Overlaps)
		return extent1;
	else if (extent1 == Enum::IntersectionType::Overlaps)
		return extent2;
	else
		return Enum::IntersectionType::Overlaps;*/
}

void Aabb::Compute(const MeshData::VertexVector& points)
{
	Min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	Max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (size_t i = 0; i < points.size(); ++i)
	{
		const Vector3& point = points[i].Position;
		Min = MinVector(Min, point);
		Max = MaxVector(Max, point);
	}
}

void Aabb::Compute(const std::vector<Vector3>& points)
{
	Min.Set(FLT_MAX, FLT_MAX, FLT_MAX);
	Max.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (size_t i = 0; i < points.size(); ++i)
	{
		const Vector3& point = points[i];
		Min = MinVector(Min, point);
		Max = MaxVector(Max, point);
	}
}

//Aabb Aabb::Compute(std::shared_ptr<Engine::ModelAsset> model)
//{
//	int meshID = model->GetMeshID();
//
//	Aabb boundingBox;
//
//	boundingBox.Compute(MeshLoader::GetMeshData(meshID)->VertexBuffer);
//
//	return boundingBox;
//}

Aabb Aabb::Transform(const Matrix3& transformation) const
{
	Vector3 translation = transformation.Translation();

	float min[3] = { Min.X, Min.Y, Min.Z };
	float max[3] = { Max.X, Max.Y, Max.Z };

	float outMin[3] = { translation.X, translation.Y, translation.Z };
	float outMax[3] = { translation.X, translation.Y, translation.Z };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			float a = transformation.Data[i][j] * min[j];
			float b = transformation.Data[i][j] * max[j];

			if (a > b)
				std::swap(a, b);

			outMin[i] += a;
			outMax[i] += b;
		}
	}

	return Aabb(
		Vector3(outMin[0], outMin[1], outMin[2], 1),
		Vector3(outMax[0], outMax[1], outMax[2], 1)
	);
}

Aabb Aabb::GetSubRegion(int region) const
{
	return Aabb();
}

bool Aabb::operator==(const Aabb& other) const
{
	return Min == other.Min && Max == other.Max;
}

bool Aabb::operator!=(const Aabb& other) const
{
	return Min != other.Min || Max != other.Max;
}