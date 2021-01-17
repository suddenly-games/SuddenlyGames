#pragma once

#include "Object.h"

namespace Engine
{
	class Environment : public Object
	{
	public:
		virtual ~Environment() {}

		void Initialize() {}

		void Update(float delta) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Environment);
	};

	//Class_Inherits(Environment, Object);
}