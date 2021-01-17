#pragma once

#include "Object.h"

namespace GraphicsEngine
{
	class Materials : public Engine::Object
	{
	public:
		virtual ~Materials() {}

		void Initialize();
		void Update(float) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Materials);
	};

}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Materials, Engine::Object);
}
