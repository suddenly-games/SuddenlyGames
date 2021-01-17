#include "Slider.h"

extern "C" {
#include <math.h>
}

Slider::Slider()
{
	Bar.SetParent(this);
	SliderFrame.SetParent(this);

	SliderFrame.Dragged.Connect([this](InputObject*) -> bool
	{
		Clamp();

		return true;
	});

	Bar.Mouse.Left.Down.Connect([this](InputObject*) -> bool
	{
		Vector3 mousePosition = GetLocalPosition(Graphics::ActiveWindow->GetMouse().Position);

		SetPercentage(GetPercentage(mousePosition) - 0.5f * GetSliderSize());

		//slider.SetPosition(DeviceVector(0, mousePosition.X, 0, mousePosition.Y));

		SliderFrame.Mouse.Left.Down.Fire(&Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MouseLeft));
		SliderFrame.Dragged.Fire(&Graphics::ActiveWindow->Input->GetInput(Enum::InputCode::MousePosition));
		SliderMoved.Fire(GetPercentage());

		return true;
	});

	Bar.Mouse.Wheel.Moved.Connect([this](InputObject* object) -> bool
	{
		Scrolled(-object->GetDelta().Y);

		return true;
	});

	SliderFrame.Mouse.Wheel.Moved.Connect([this](InputObject* object) -> bool
	{
		Scrolled(-object->GetDelta().Y);

		return true;
	});
}

void Slider::SetDirection(Enum::SliderDirection newDirection)
{
	float size = GetSliderSize();
	float percentage = GetPercentage();

	Direction = newDirection;

	SetSliderSize(size);
	SetPercentage(percentage);

	Clamp();
}

void Slider::SetSliderSize(float percentage)
{
	IncrementsLocked = false;

	if (Direction == Enum::SliderDirection::Horizontal)
		SliderFrame.SetSize(DeviceVector(percentage, 0, 1, 0));
	else
		SliderFrame.SetSize(DeviceVector(1, 0, percentage, 0));

	Clamp();
}

void Slider::SetPercentage(float percentage)
{
	Vector3 max = GetAbsoluteSize() - SliderFrame.GetAbsoluteSize();

	if (Direction == Enum::SliderDirection::Horizontal)
		SliderFrame.SetPosition(DeviceVector(0, percentage * max.X, 0, 0));
	else
		SliderFrame.SetPosition(DeviceVector(0, 0, 0, percentage * max.Y));

	Clamp();
}

void Slider::SetSliderIncrements(int sliderSize, int totalSize)
{
	IncrementsLocked = true;
	
	SliderIncrementSize = sliderSize;
	TotalIncrements = totalSize;

	if (Direction == Enum::SliderDirection::Horizontal)
		SliderFrame.SetSize(DeviceVector(float(sliderSize) / float(totalSize), 0, 1, 0));
	else
		SliderFrame.SetSize(DeviceVector(1, 0, float(sliderSize) / float(totalSize), 0));

	Clamp();
}

void Slider::Scrolled(float ticks)
{
	if (IncrementsLocked)
		SetPercentage(GetPercentage() + ticks / float(TotalIncrements - SliderIncrementSize));
	else
		SetPercentage(GetPercentage() + ticks * ScrollSpeed);
}

float Slider::GetPercentage() const
{
	return GetPercentage(SliderFrame.GetLocalPosition());
}

float Slider::GetPercentage(const Vector3 position) const
{
	Vector3 max = GetAbsoluteSize() - SliderFrame.GetAbsoluteSize();

	if (Direction == Enum::SliderDirection::Horizontal)
		return max == 0 ? 0 : position.X / max.X;
	else
		return max == 0 ? 0 : position.Y / max.Y;
}

float Slider::GetSliderSize() const
{
	Vector3 size = GetAbsoluteSize();

	if (Direction == Enum::SliderDirection::Horizontal)
		return size.X == 0 ? 1 : SliderFrame.GetAbsoluteSize().X / size.X;
	else
		return size.Y == 0 ? 1 : SliderFrame.GetAbsoluteSize().Y / size.Y;
}

bool Slider::SliderIncrementsLocked() const
{
	return IncrementsLocked;
}

int Slider::GetSliderIncrementSize() const
{
	return SliderIncrementSize;
}

int Slider::GetTotalIncrements() const
{
	return TotalIncrements;
}

Enum::SliderDirection Slider::GetDirection() const
{
	return Direction;
}

void Slider::Clamp()
{
	Vector3 position = SliderFrame.GetLocalPosition();
	Vector3 max = GetAbsoluteSize() - SliderFrame.GetAbsoluteSize();

	if (Direction == Enum::SliderDirection::Horizontal)
	{
		position.Y = 0;

		Clamp(position.X, max.X);
	}
	else
	{
		position.X = 0;

		Clamp(position.Y, max.Y);
	}

	SliderFrame.SetPosition(DeviceVector(0, position.X, 0, position.Y));

	SliderMoved.Fire(GetPercentage());
}

void Slider::Clamp(float& x, float max)
{
	if (x > max)
		x = max;

	if (x < 0)
		x = 0;

	if (IncrementsLocked)
	{
		int maxIncrements = TotalIncrements - SliderIncrementSize;
	
		x = floor((x / max) * float(maxIncrements)) / float(maxIncrements) * max;
	}
}

void Slider::UpdateFrame()
{
	Clamp();
}
