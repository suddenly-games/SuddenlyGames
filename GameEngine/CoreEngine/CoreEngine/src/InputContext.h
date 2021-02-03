#pragma once

#include "Object.h"

namespace GraphicsEngine
{
	class DeviceTransform;
	class InputSubscriber;

	class InputContext : public Engine::Object
	{
	public:
		bool Enabled = true;
		std::weak_ptr<InputSubscriber> MouseFocus;
		std::weak_ptr<InputSubscriber> InputFocus;

		static std::weak_ptr<InputContext> CurrentContext;

		void Initialize() {}
		void Update(float);

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputContext);
	};
}