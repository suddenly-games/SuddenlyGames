#include "ScrollBar.h"

ScrollBar::ScrollBar()
{
	Decrease.SetParent(this);
	Increase.SetParent(this);
	ScrollSlider.SetParent(this);
	Decrease.SetAlignment(Enum::Alignment::Minimum, Enum::Alignment::Minimum);
	Increase.SetAlignment(Enum::Alignment::Maximum, Enum::Alignment::Maximum);
	ScrollSlider.SetAlignment(Enum::Alignment::Center, Enum::Alignment::Center);

	TaskScheduler::Timer(eachFrame, [this](float delta, float time, int)
	{
		float direction = 0;

		if (Increase.Mouse.Left.IsDown)
			direction = 1;
		else if (Decrease.Mouse.Left.IsDown)
			direction = -1;

		if (ScrollSlider.SliderIncrementsLocked())
		{
			if (time < lastIncrement)
				direction = 0;
			else
			{
				direction *= ScrollSlider.GetSliderSize();
				lastIncrement += ButtonScrollSpeed;
			}
		}
		else
			direction *= ButtonScrollSpeed * ScrollSlider.GetSliderSize() * delta;

		ScrollSlider.SetPercentage(ScrollSlider.GetPercentage() + direction);
	});
}

void ScrollBar::SetDirection(Enum::SliderDirection newDirection)
{
	Direction = newDirection;

	ScrollSlider.SetDirection(newDirection);

	SetButtonSize(ButtonSize);
}

void ScrollBar::SetButtonSize(float size)
{
	ButtonSize = size;

	DeviceVector buttonSize(0, 0, 0, 0);
	DeviceVector sliderSize(0, 0, 0, 0);

	if (Direction == Enum::SliderDirection::Horizontal)
	{
		buttonSize = DeviceVector(size, 0, 1, 0);
		sliderSize = DeviceVector(1 - 2 * size, 0, 1, 0);
	}
	else
	{
		buttonSize = DeviceVector(1, 0, size, 0);
		sliderSize = DeviceVector(1, 0, 1 - 2 * size, 0);
	}

	Increase.SetSize(buttonSize);
	Decrease.SetSize(buttonSize);
	ScrollSlider.SetSize(sliderSize);
}

Enum::SliderDirection ScrollBar::GetDirection() const
{
	return Direction;
}

float ScrollBar::GetButtonSize() const
{
	return ButtonSize;
}
