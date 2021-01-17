#pragma once

#include "Object.h"
#include "Vector3.h"

namespace Engine
{
	namespace Physics
	{
		class PointMass : public Object
		{
		public:
			~PointMass() {}

			float Mass = 1;
			Vector3 Position;

			void Initialize() {}
			void Update(float) {}

			Instantiable;

			Inherits_Class(Object);

			Reflected(PointMass);
		};
	}

	//Class_Inherits(Physics::PointMass, Object);
}