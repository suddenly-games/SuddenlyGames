#pragma once

#include "RenderOperation.h"
#include "Camera.h"
#include "Scene.h"

namespace GraphicsEngine
{
	class FrameBuffer;
	class Camera;
	class Scene;
	class Light;

	class ShadingOperation : public RenderOperation
	{
	public:
		std::weak_ptr<FrameBuffer> SceneBuffer;
		std::weak_ptr<FrameBuffer> LightBuffer;
		std::weak_ptr<Camera> CurrentCamera;
		std::weak_ptr<Scene> CurrentScene;
		std::weak_ptr<Light> GlobalLight;
		Vector3 Resolution;

		void Initialize();
		void Update(float);
		void Render();

	private:
		std::weak_ptr<FrameBuffer> RightMap;
		std::weak_ptr<FrameBuffer> LeftMap;
		std::weak_ptr<FrameBuffer> TopMap;
		std::weak_ptr<FrameBuffer> BottomMap;
		std::weak_ptr<FrameBuffer> FrontMap;
		std::weak_ptr<FrameBuffer> BackMap;
		std::weak_ptr<Camera> ShadowCamera;
		std::weak_ptr<Scene> ShadowScene;

		AabbTree UpdatedLights;

		void Draw(const std::shared_ptr<Light>& light);
		void DrawShadows(const std::shared_ptr<Light>& light, int index);

		Instantiable;

		Inherits_Class(RenderOperation);

		Reflected(ShadingOperation);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::ShadingOperation, GraphicsEngine::RenderOperation);
}
