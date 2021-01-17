#include "Button.h"

const AppearanceData* Button::GetCurrentAppearance() const
{
	bool mouseDown = Mouse.Left.IsDown || Mouse.Wheel.IsDown || Mouse.Right.IsDown;

	if (mouseDown)
		return &ClickAppearance;
	else if (MouseHovering)
		return &HoverAppearance;
	else
		return &Appearance;
}
