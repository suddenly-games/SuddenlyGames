#pragma once

#include "Object.h"

namespace GraphicsEngine
{
	class Texture;
	class FrameBuffer;

	class CanvasStencil : public Engine::Object
	{
	public:
		bool Enabled = true;

		~CanvasStencil();

		void Initialize();
		void Update(float);

		void Draw();
		std::shared_ptr<Texture> GetTexture();

	private:
		std::weak_ptr<Texture> StencilTexture;
		std::weak_ptr<FrameBuffer> StencilBuffer;

		Instantiable;

		Inherits_Class(Object);

		Reflected(CanvasStencil);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::CanvasStencil, Object);
}