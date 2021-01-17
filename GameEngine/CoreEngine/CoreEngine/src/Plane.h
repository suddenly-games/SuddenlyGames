#pragma once

#include "Vector3.h"

class Plane : public Vector3
{
public:
	void Set(const Vector3& position, const Vector3& normal);

	struct IntersectionTypeEnum
	{
		enum IntersectionType
		{
			Inside,
			Outside,
			Overlaps,
			Coplanar
		};
	};

	typedef IntersectionTypeEnum::IntersectionType IntersectionType;

	IntersectionType Compare(const Vector3& point, float epsilon = 1e-5f) const;
};

namespace Enum
{
	typedef Plane::IntersectionType IntersectionType;
}