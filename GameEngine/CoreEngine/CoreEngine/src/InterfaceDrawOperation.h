#pragma once

#include "RenderOperation.h"
#include "DeviceTransform.h"

namespace GraphicsEngine
{
	class InterfaceDrawOperation : public RenderOperation
	{
	public:
		std::weak_ptr<DeviceTransform> CurrentScreen;

		void Initialize() {}
		void Update(float) {}

		void Render();

		Instantiable;

		Inherits_Class(RenderOperation);

		Reflected(InterfaceDrawOperation);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::InterfaceDrawOperation, GraphicsEngine::RenderOperation);
}
