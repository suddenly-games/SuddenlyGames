#pragma once

#include "Button.h"

class CheckBox : public Button
{
public:
	AppearanceData CheckedAppearance;
	AppearanceData CheckedHoverAppearance;
	AppearanceData CheckedClickAppearance;
	AppearanceData UncheckedAppearance;
	AppearanceData UncheckedHoverAppearance;
	AppearanceData UncheckedClickAppearance;

	Event<bool> ValueChanged;

	CheckBox();

	void SetChecked(bool value);
	void RefreshAppearance();
	bool IsChecked() const;

private:
	bool Checked = false;
};
