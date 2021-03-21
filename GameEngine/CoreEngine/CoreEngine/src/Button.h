#pragma once

#include "Object.h"
#include "InputEnums.h"

namespace GraphicsEngine
{
	class Appearance;
	class InputSubscription;

	class Button : public Engine::Object
	{
	public:
		std::weak_ptr<Appearance> Hover;
		std::weak_ptr<Appearance> Pressed;
		std::weak_ptr<Appearance> Idle;

		std::weak_ptr<InputSubscription> Binding;

		Enum::BoundDevice Device = Enum::BoundDevice::Any;

		void Initialize() {}
		void Update(float);

	private:
		bool Down = false;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Button);
	};
}