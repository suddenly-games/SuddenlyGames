#include "DrawSceneOperation.h"

#include "Graphics.h"

namespace GraphicsEngine
{
	void DrawSceneOperation::Configure(const std::shared_ptr<Scene>& scene)
	{
		CurrentScene = scene;
	}

	void DrawSceneOperation::Render()
	{
		Graphics::SetClearColor(RGBA(0x000000FF));
		Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CurrentScene.lock()->Draw();
	}
}
