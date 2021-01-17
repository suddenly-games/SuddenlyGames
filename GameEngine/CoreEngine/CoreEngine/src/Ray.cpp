#include "Ray.h"

#include <limits>

float minf(float a, float b)
{
	return a < b ? a : b;
}

float maxf(float a, float b)
{
	return a > b ? a : b;
}

bool Ray::Intersects(const Aabb& box, float& t, float epsilon) const
{
	float tMax = std::numeric_limits<float>::max();
	float tMin = -tMax;

	if (!(
		IntersectsAxis(Start.X, Direction.X, box.Min.X, box.Max.X, tMin, tMax, epsilon) &&
		IntersectsAxis(Start.Y, Direction.Y, box.Min.Y, box.Max.Y, tMin, tMax, epsilon) &&
		IntersectsAxis(Start.Z, Direction.Z, box.Min.Z, box.Max.Z, tMin, tMax, epsilon)
	) || tMax < 0)
		return false;

	t = maxf(0, tMin);

	return true;
}

bool Ray::IntersectsAxis(float start, float direction, float min, float max, float& tMin, float& tMax, float epsilon) const
{
	if (direction < -epsilon || direction > epsilon)
	{
		float t1 = (min - start) / direction;
		float t2 = (max - start) / direction;

		if (t1 > t2)
		{
			tMin = maxf(tMin, t2);
			tMax = minf(tMax, t1);
		}
		else
		{
			tMin = maxf(tMin, t1);
			tMax = minf(tMax, t2);
		}
	}
	else
		return start >= min && start <= max;

	return tMin <= tMax;
}