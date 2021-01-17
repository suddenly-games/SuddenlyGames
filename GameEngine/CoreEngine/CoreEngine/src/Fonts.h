#pragma once

#include "Font.h"

#include <vector>
#include <map>

#include "Object.h"

namespace GraphicsEngine
{
	class Fonts : public Engine::Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Fonts);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Fonts, Engine::Object);
}
