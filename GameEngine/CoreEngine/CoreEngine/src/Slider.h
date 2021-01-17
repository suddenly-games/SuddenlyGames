#pragma once

#include "DraggableFrame.h"
#include "OptionSlider.h"

class Slider : public FrameBase
{
public:
	Frame Bar;
	DraggableFrame SliderFrame;

	float ScrollSpeed = 0.1f;

	Event<float> SliderMoved;

	Slider();

	void SetDirection(Enum::SliderDirection newDirection);
	void SetSliderSize(float percentage);
	void SetPercentage(float percentage);
	void SetSliderIncrements(int sliderSize, int totalSize);
	void Scrolled(float ticks);
	float GetPercentage() const;
	float GetPercentage(const Vector3 position) const;
	float GetSliderSize() const;
	bool SliderIncrementsLocked() const;
	int GetSliderIncrementSize() const;
	int GetTotalIncrements() const;
	Enum::SliderDirection GetDirection() const;
	void UpdateFrame();

private:
	Enum::SliderDirection Direction = Enum::SliderDirection::Vertical;
	bool IncrementsLocked = false;
	int TotalIncrements = 1;
	int SliderIncrementSize = 1;

	void Clamp();
	void Clamp(float& x, float max);
};
