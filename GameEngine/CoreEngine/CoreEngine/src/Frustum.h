#pragma once

#include "Vector3.h"
#include "Aabb.h"
#include "Plane.h"

class Frustum
{
public:
	Plane Planes[6];

	Enum::IntersectionType Intersects(const Aabb& box, float epsilon = 1e-5f, int* lastSide = nullptr) const;
};