#pragma once

#include "Vector3.h"
#include "Matrix3.h"
#include "ObjectReflection.h"
#include "MeshData.h"
#include "Plane.h"

class Aabb
{
public:
	struct AxisEnum
	{
		enum Axis
		{
			MinX = 0,
			MinY = 0,
			MinZ = 0,

			MaxX = 1,
			MaxY = 2,
			MaxZ = 4
		};
	};

	typedef AxisEnum::Axis Axis;

	Vector3 Min;
	Vector3 Max;

	Aabb(const Vector3& min = Vector3(), const Vector3& max = Vector3());

	void Expand(const Vector3& point);
	bool ContainsPoint(const Vector3& point) const;
	bool Intersects(const Aabb& box) const;
	bool InRange(float x, float min, float max) const;
	Vector3 GetCenter() const;
	Vector3 GetSize() const;
	Aabb Transform(const Matrix3& transformation) const;
	Aabb GetSubRegion(int region) const;

	Enum::IntersectionType Intersects(const Plane& plane, float epsilon = 1e-5f) const;

	// To easily make a Aabb from a set of points, helps with the implementation of GJK
	void Compute(const MeshData::VertexVector& points);
	void Compute(const std::vector<Vector3>& points);
	//static Aabb Compute(Handle<Engine::ModelAsset> model);

	bool operator==(const Aabb& other) const;
	bool operator!=(const Aabb& other) const;

	Base_Class;

	Reflected_Type(Aabb);
};

namespace Enum
{
	typedef Aabb::Axis Axis;
}

namespace Engine
{
	////Class_Inherits(Aabb, CoreType);

	Define_Value_Type(Aabb);
}
