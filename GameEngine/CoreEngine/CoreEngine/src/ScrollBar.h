#pragma once

#include "Slider.h"
#include "Button.h"

class ScrollBar : public FrameBase
{
public:
	Button Increase;
	Slider ScrollSlider;
	Button Decrease;

	float ButtonScrollSpeed = 1;

	ScrollBar();

	void SetDirection(Enum::SliderDirection newDirection);
	void SetButtonSize(float size);
	Enum::SliderDirection GetDirection() const;
	float GetButtonSize() const;

private:
	float lastIncrement = Graphics::ActiveWindow->GetTime();
	float ButtonSize = 0.05f;
	Enum::SliderDirection Direction = Enum::SliderDirection::Vertical;
};
