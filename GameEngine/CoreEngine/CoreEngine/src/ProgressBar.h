#pragma once

#include "Frame.h"
#include "DraggableFrame.h"
#include "OptionSlider.h"

class ProgressBar : public FrameBase
{
public:
	Frame Container;
	Frame Bar;

	Event<float> ValueChanged;

	ProgressBar();

	void SetDirection(Enum::SliderDirection newDirection);
	void SetPercentage(float newPercentage);
	void SetReverseDirection(bool reverseDirection);
	float GetPercentage() const;
	Enum::SliderDirection GetDirection() const;
	bool GetReverseDirection() const;

private:
	Enum::SliderDirection Direction = Enum::SliderDirection::Vertical;
	float Percentage = 0;
	bool Reverse = false;

	void UpdateBar();
};
