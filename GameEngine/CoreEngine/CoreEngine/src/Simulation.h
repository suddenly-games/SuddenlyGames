#pragma once

#include "Object.h"

namespace Engine
{
	class Simulation : public Object
	{
	public:
		virtual ~Simulation() {}

		void Initialize() {}

		void Update(float delta) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Simulation);
	};

	//Class_Inherits(Simulation, Object);
}