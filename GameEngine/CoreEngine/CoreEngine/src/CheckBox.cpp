#include "CheckBox.h"

CheckBox::CheckBox()
{
	Appearance = UncheckedAppearance;
	HoverAppearance = UncheckedHoverAppearance;
	ClickAppearance = UncheckedClickAppearance;

	Mouse.Left.Up.Connect([this](InputObject*) -> bool
	{
		SetChecked(!Checked);

		return true;
	});
}

void CheckBox::SetChecked(bool value)
{
	Checked = value;

	RefreshAppearance();

	ValueChanged.Fire(value || false);
}

void CheckBox::RefreshAppearance()
{
	if (Checked)
	{
		Appearance = CheckedAppearance;
		HoverAppearance = CheckedHoverAppearance;
		ClickAppearance = CheckedClickAppearance;
	}
	else
	{
		Appearance = UncheckedAppearance;
		HoverAppearance = UncheckedHoverAppearance;
		ClickAppearance = UncheckedClickAppearance;
	}
}

bool CheckBox::IsChecked() const
{
	return Checked;
}
