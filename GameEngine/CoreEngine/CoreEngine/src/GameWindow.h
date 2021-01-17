#pragma once

#include "Object.h"
#include "Window.h"

namespace GraphicsEngine
{
	class GameWindow : public Engine::Object
	{
	public:
		void Initialize() {}
		void Update(float) {}

		void Configure(Window* window);

		int GetRefreshRate();
		Vector3 GetResolution();
		void SetMousePosition(const Vector3& position);

	private:
		Window* ActiveWindow = nullptr;

		Instantiable;

		Inherits_Class(Object);

		Reflected(GameWindow);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::GameWindow, Object);
}