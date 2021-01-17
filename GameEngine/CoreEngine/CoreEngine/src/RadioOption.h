#pragma once

#include <string>

#include "CheckBox.h"
#include "RadioContainer.h"

class RadioOption : public CheckBox
{
public:
	RadioOption(int value);

	void SetValue(int newValue);
	void SetContainer(RadioContainer* newContainer);
	int GetValue() const;
	RadioContainer* GetContainer();

private:
	int Value;
	RadioContainer* Container = nullptr;
};
