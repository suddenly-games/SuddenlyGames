#pragma once

#include "FrameBase.h"

class RadioContainer : public FrameBase
{
public:
	Event<int> ValueChanged;

	void SetValue(int newValue);
	int GetValue() const;

private:
	int Value = -1;
};
