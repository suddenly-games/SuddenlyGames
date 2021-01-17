#pragma once

#include "Appearance.h"

namespace GraphicsEngine
{
	class DeviceTransform;

	class ScreenCanvas : public Engine::Object
	{
	public:
		bool Visible = true;
		std::weak_ptr<Appearance> Appearance;

		~ScreenCanvas() {}

		void Initialize() {}
		void Update(float) {}

		void Draw();
		
	private:
		void DrawScreen(const std::shared_ptr<DeviceTransform>& transform);
	
		Instantiable;

		Inherits_Class(Object);

		Reflected(ScreenCanvas);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::ScreenCanvas, Object);
}