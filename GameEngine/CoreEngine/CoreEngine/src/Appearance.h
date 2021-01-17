#pragma once

#include "Object.h"
#include "RGBA.h"
#include "Vector3.h"

namespace GraphicsEngine
{
	class Texture;

	class Appearance : public Engine::Object
	{
	public:
		bool BlendTexture = true;
		RGBA Color;
		RGBA TextureColor;
		Vector3 UVScale = Vector3(1, 1);
		Vector3 UVOffset = Vector3(0, 0);
		std::weak_ptr<Texture> Texture;

		~Appearance() {}

		void Initialize() {}
		void Update(float) {}

		Instantiable;

		Inherits_Class(Object);

		Reflected(Appearance);
	};
}

namespace Engine
{
	////Class_Inherits(GraphicsEngine::Appearance, Object);
}