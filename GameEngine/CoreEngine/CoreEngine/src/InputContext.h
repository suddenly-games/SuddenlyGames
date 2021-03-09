#pragma once

#include "Object.h"
#include "Vector3.h"
#include "InputEnums.h"

namespace Engine
{
	class UserInput;
}

namespace GraphicsEngine
{
	class DeviceTransform;
	class InputSubscriber;

	class InputContext : public Engine::Object
	{
	public:
		bool Enabled = true;

		std::weak_ptr<InputSubscriber> Focuses[Enum::BoundDevice::Count] = {};

		std::weak_ptr<DeviceTransform> Device;
		std::weak_ptr<Engine::UserInput> InputSource;

		std::shared_ptr<InputSubscriber> GetFocus(Enum::BoundDevice device) const;
		void SetFocus(Enum::BoundDevice device, const std::shared_ptr<InputSubscriber>& subscriber);

		void Initialize() {}
		void Update(float);

	private:

		std::shared_ptr<InputSubscriber> GetMouseFocus(const Vector3& mousePosition, const std::shared_ptr<Object>& object) const;

		Instantiable;

		Inherits_Class(Object);

		Reflected(InputContext);
	};
}