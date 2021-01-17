#pragma once

#include "Object.h"

namespace Engine
{
	class Environments : public Object
	{
	public:
		virtual ~Environments() {}

		void Initialize() {}

		void Update(float delta) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Environments);
	};

	//Class_Inherits(Environments, Object);
}