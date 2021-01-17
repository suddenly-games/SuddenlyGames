#pragma once

#include <string>
#include <vector>
#include <map>

#include "RGBA.h"
#include "Object.h"

namespace GraphicsEngine
{
	class Material : public Engine::Object
	{
	public:
		RGBA Diffuse;
		RGBA Specular;
		RGBA Ambient;
		RGBA Emission;
		int Shininess;

		void Initialize() {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Material);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Material, Object);
}
