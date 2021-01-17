#include "PointMass.h"

namespace Engine
{
	using Physics::PointMass;

	Reflect_Inherited(PointMass, Object,
		Document_Class("");
			
		Document("");
		Archivable Class_Member(float, Mass);
			
		Document("");
		Archivable Class_Member(Vector3, Position);
	);
}