#include "OptionSlider.h"

OptionSlider::OptionSlider()
{
	BackgroundContainer.SetParent(this);
	OptionContainer.SetParent(this);
	OptionLabelContainer.SetParent(this);
	OptionLabelContainer.InputEnabled = false;
	Slider.SetParent(&OptionContainer);

	TaskScheduler::Timer(eachFrame, [this](float delta, float, int)
	{
		float option = float(CurrentOption);
		float change = SliderSpeed * delta;
		DeviceVector position = GetSlider()->GetPosition();
		float& axis = GetDirection() == Enum::SliderDirection::Horizontal ? position.X.Scale : position.Y.Scale;
		bool snapToPoint = false;

		//if (getSlider()->Mouse.Left.IsDown)
		//	change *= 5;

		if (option < axis)
		{
			change *= -1;
			snapToPoint = (option - axis) > change;
		}
		else
			snapToPoint = (option - axis) < change;

		if (snapToPoint)
			axis = option;
		else
			axis += change;

		GetSlider()->SetPosition(position);
	});

	Slider.Mouse.Wheel.Moved.Connect([this](InputObject* input) -> bool
	{
		Scrolled(int(input->GetDelta().Y));

		return true;
	});
}

void OptionSlider::SetOptions(StringArray optionNames)
{
	ClearOptions();

	Slider.SetParent(nullptr);

	Vector3 coords;

	for (int i = 0; optionNames[i] != nullptr; ++i)
	{
		coords.Set(Direction == SliderDirection::Horizontal ? float(i) : 0, Direction == SliderDirection::Vertical ? float(i) : 0);

		Button* button = new Button();
		button->SetPosition(DeviceVector(coords.X, 0, coords.Y, 0));
		button->SetParent(&OptionContainer);
		button->Mouse.Left.Up.Connect([this, i](InputObject*) -> bool
		{
			CurrentOption = i;

			ValueChanged.Fire(i + 0);

			return true;
		});
		button->Mouse.Wheel.Moved.Connect([this](InputObject* input) -> bool
		{
			Scrolled(int(input->GetDelta().Y));

			return true;
		});
		button->MouseEntered.Connect([this, i]() -> bool
		{
			if (GetSlider()->Mouse.Left.IsDown)
			{
				CurrentOption = i;
				ValueChanged.Fire(i + 0);
			}

			return true;
		});

		TextFrame* label = new TextFrame();
		label->SetPosition(DeviceVector(coords.X, 0, coords.Y, 0));
		label->SetParent(&OptionLabelContainer);
		label->FontSize = FontSize;
		label->Text.Color = FontColor;
		label->Text.SetFont(FontType);
		label->Text.SetText(optionNames[i]);

		Options.push_back(button);
		OptionLabels.push_back(label);
	}

	DeviceVector size(0, 0, 0, 0);

	if (Direction == SliderDirection::Horizontal)
		size = DeviceVector(1 / float(Options.size()), 0, 1, 0);
	else
		size = DeviceVector(1, 0, 1 / float(Options.size()), 0);

	OptionContainer.SetSize(size);
	OptionLabelContainer.SetSize(size);

	if (LockOptionAspectRatio)
	{
		if (Direction == SliderDirection::Horizontal)
			SetAspectRatio(OptionAspectRatio * float(GetOptionCount()));
		else
			SetAspectRatio(OptionAspectRatio / float(GetOptionCount()));
	}

	if (Direction == SliderDirection::Horizontal)
		SetSize(DeviceVector(float(Options.size()) * OptionSize.X, OptionSize.Y));
	else
		SetSize(DeviceVector(OptionSize.X, float(Options.size()) * OptionSize.Y));

	if (CurrentOption > int(Options.size()))
		CurrentOption = int(Options.size()) - 1;

	if (CurrentOption < 0)
		CurrentOption = 0;

	Slider.SetParent(&OptionContainer);

	RefreshAppearance();
}

void OptionSlider::SetDirection(SliderDirection newDirection)
{
	Direction = newDirection;

	RefreshAppearance();
}

void OptionSlider::SetOptionSize(const DeviceVector& newOptionSize)
{
	OptionSize = newOptionSize;

	RefreshAppearance();
}

void OptionSlider::SetOptionAspectRatio(float aspectRatio)
{
	OptionAspectRatio = aspectRatio;
	LockOptionAspectRatio = true;

	SetAspectRatio(OptionAspectRatio / float(GetOptionCount()));
}

void OptionSlider::SetOptionAspectRatioLocked(bool locked)
{
	LockOptionAspectRatio = locked;

	SetAspectRatioLocked(false);
}

void OptionSlider::RefreshAppearance()
{
	for (int i = 0; i < int(Options.size()); ++i)
	{
		Options[i]->Appearance = OptionAppearance;
		Options[i]->HoverAppearance = OptionHoverAppearance;
		Options[i]->ClickAppearance = OptionClickAppearance;
		OptionLabels[i]->Text.Color = FontColor;
		OptionLabels[i]->FontSize = FontSize;
	}

	Slider.Appearance = SliderAppearance;
	Slider.HoverAppearance = SliderHoverAppearance;
	Slider.ClickAppearance = SliderClickAppearance;
}

void OptionSlider::Scrolled(int ticks)
{
	CurrentOption -= ticks;

	int optionCount = GetOptionCount();

	if (CurrentOption >= optionCount)
		CurrentOption = optionCount - 1;

	if (CurrentOption < 0)
		CurrentOption = 0;

	ValueChanged.Fire(CurrentOption + 0);
}

FrameBase* OptionSlider::GetBackgroundContainer()
{
	return &BackgroundContainer;
}

std::string OptionSlider::GetCurrentOptionName() const
{
	if (CurrentOption >= 0 && CurrentOption < int(Options.size()))
		return OptionLabels[CurrentOption]->Text.GetText();

	return "";
}

std::string OptionSlider::GetOptionName(int id) const
{
	if (id >= 0 && id < int(Options.size()))
		return OptionLabels[id]->Text.GetText();

	return "";
}

int OptionSlider::GetOptionCount() const
{
	return int(Options.size());
}

Button* OptionSlider::GetSlider()
{
	return &Slider;
}

Enum::SliderDirection OptionSlider::GetDirection() const
{
	return Direction;
}

void OptionSlider::ClearOptions()
{
	for (int i = int(Options.size()) - 1; i >= 0; --i)
	{
		delete Options[i];
		delete OptionLabels[i];
	}

	Options.clear();
	OptionLabels.clear();
}

void OptionSlider::UpdateFrame()
{
	if (BackgroundContainer.GetParent() != this)
		BackgroundContainer.SetParent(this);
}
