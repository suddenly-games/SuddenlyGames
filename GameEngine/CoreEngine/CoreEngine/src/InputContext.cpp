#include "InputContext.h"

#include "DeviceTransform.h"
#include "LuaInput.h"
#include "Aabb.h"
#include "InputSubscriber.h"

namespace GraphicsEngine
{

	std::shared_ptr<InputSubscriber> InputContext::GetFocus(Enum::BoundDevice device) const
	{
		return Focuses[device].lock();
	}

	void InputContext::SetFocus(Enum::BoundDevice device, const std::shared_ptr<InputSubscriber>& subscriber)
	{
		Focuses[device] = subscriber;
	}

	void InputContext::Update(float)
	{
		if (!Enabled && Device.expired())
			return;

		std::shared_ptr<DeviceTransform> device = Device.lock();

		std::shared_ptr<Engine::UserInput> input = InputSource.lock();

		Vector3 mousePosition = input->GetPosition(Enum::InputCode::MousePosition);
		Vector3 resolution = device->GetResolution();

		mousePosition.Scale(2 / resolution.X, 2 / resolution.Y, 1) += Vector3(-1, -1, 0);

		mousePosition.Y *= -1;

		if (!Focuses[Enum::BoundDevice::Mouse1].expired())
			Focuses[Enum::BoundDevice::Mouse1].lock()->SetFocus(Enum::BoundDevice::Mouse1, false);

		std::shared_ptr<InputSubscriber> subscriber = GetMouseFocus(mousePosition + Vector3(0, 0, 0, 1), Device.lock());

		if (subscriber != nullptr)
			subscriber->SetFocus(Enum::BoundDevice::Mouse1, true);

		Focuses[Enum::BoundDevice::Mouse1] = subscriber;
	}

	const Aabb standardSquare = Aabb(Vector3(-1, -1, -1), Vector3(1, 1, 1));

	std::shared_ptr<InputSubscriber> InputContext::GetMouseFocus(const Vector3& mousePosition, const std::shared_ptr<Object>& object) const
	{
		int a = 0;
		if (object->Name == "ButtonTransform")
			++a;

		if (object->IsA<DeviceTransform>())
		{
			Vector3 localPosition = object->Cast<DeviceTransform>()->GetInverseTransformation() * mousePosition;

			if (!standardSquare.ContainsPoint(localPosition))
				return nullptr;
		}

		std::shared_ptr<InputSubscriber> focus = object->Get<InputSubscriber>();//object->IsA<InputSubscriber>() ? object->Cast<InputSubscriber>() : nullptr;

		for (int i = 0; i < object->GetChildren(); ++i)
		{
			std::shared_ptr<InputSubscriber> newFocus = GetMouseFocus(mousePosition, object->Get(i)->Cast<Engine::Object>());

			focus = newFocus != nullptr ? newFocus : focus;
		}
		
		return focus;
	}
}