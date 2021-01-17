#include "Plane.h"

void Plane::Set(const Vector3& position, const Vector3& normal)
{
	Vector3 normalized = normal.Unit();
	Vector3::Set(normalized.X, normalized.Y, normalized.Z, position * normalized);
}

typename Enum::IntersectionType Plane::Compare(const Vector3& point, float epsilon) const
{
	float projection = Dot(Vector3(point.X, point.Y, point.Z, -1));

	if (projection > epsilon)
		return IntersectionType::Inside;
	else if (projection < -epsilon)
		return IntersectionType::Outside;
	else
		return IntersectionType::Coplanar;
}