#include "Frustum.h"

Enum::IntersectionType Frustum::Intersects(const Aabb& box, float epsilon, int* lastSide) const
{
	bool overlaps = false;
	int start = 0;

	if (lastSide != nullptr)
		start = *lastSide;

	for (int i = 0; i < 6; ++i)
	{
		const Plane& plane = Planes[(start + i) % 6];

		Enum::IntersectionType type = box.Intersects(plane, epsilon);

		overlaps |= type == Enum::IntersectionType::Overlaps;

		if (type == Enum::IntersectionType::Outside)
		{
			if (lastSide != nullptr)
				*lastSide = (*lastSide + i) % 6;

			return Enum::IntersectionType::Outside;
		}
	}

	return overlaps ? Enum::IntersectionType::Overlaps : Enum::IntersectionType::Inside;
}