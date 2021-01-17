#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	Container.SetParent(this);
	Bar.SetParent(this);
}

void ProgressBar::SetDirection(Enum::SliderDirection newDirection)
{
	Direction = newDirection;

	UpdateBar();
}

void ProgressBar::SetPercentage(float newPercentage)
{
	Percentage = newPercentage;

	ValueChanged.Fire(newPercentage + 0);

	UpdateBar();
}

float ProgressBar::GetPercentage() const
{
	return Percentage;
}

Enum::SliderDirection ProgressBar::GetDirection() const
{
	return Direction;
}

void ProgressBar::UpdateBar()
{
	if (Direction == Enum::SliderDirection::Horizontal)
		Bar.SetSize(DeviceVector(Percentage, 0, 1, 0));
	else
		Bar.SetSize(DeviceVector(1, 0, Percentage, 0));
}
