#pragma once

#include "RenderOperation.h"
#include "Scene.h"

namespace GraphicsEngine
{
	class DrawSceneOperation : public RenderOperation
	{
	public:
		std::weak_ptr<Scene> CurrentScene;

		void Initialize() {}
		void Update(float) {}
		void Configure(const std::shared_ptr<Scene>& scene);

		void Render();

		Instantiable;

		Inherits_Class(RenderOperation);

		Reflected(DrawSceneOperation);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::DrawSceneOperation, GraphicsEngine::RenderOperation);
}
