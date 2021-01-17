#pragma once

#include "Vector3.h"
#include "Aabb.h"
#include "ObjectReflection.h"

class Ray
{
public:
	Vector3 Start;
	Vector3 Direction;

	Ray() {}
	Ray(const Vector3& start, const Vector3& direction) : Start(start), Direction(direction) {}

	bool Intersects(const Aabb& box, float& t, float epsilon = 1e-5f) const;

private:
	bool IntersectsAxis(float start, float direction, float min, float max, float& tMin, float& tMax, float epsilon = 1e-5f) const;

	Base_Class;

	Reflected_Type(Ray);
};

namespace Engine
{
	//Class_Inherits(Ray, CoreType);

	Define_Value_Type(Ray);
}
