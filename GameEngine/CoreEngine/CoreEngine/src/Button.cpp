#include "Button.h"

#include "InputSubscriber.h"
#include "Appearance.h"
#include "ScreenCanvas.h"

namespace GraphicsEngine
{
	void Button::Update(float)
	{
		std::shared_ptr<ScreenCanvas> canvas = GetComponent<ScreenCanvas>();

		if (canvas == nullptr)
			return;

		std::shared_ptr<InputSubscription> input = Binding.lock();

		if (input == nullptr)
			return;

		Enum::InputState state = input->GetStateEnum();

		Down = (Down && state != Enum::InputState::Idle && state != Enum::InputState::Ended) || state == Enum::InputState::Began;

		if (Down)
			canvas->Appearance = Pressed.lock();
		else if (input->HasFocus(Device))
			canvas->Appearance = Hover.lock();
		else
			canvas->Appearance = Idle.lock();
	}
}