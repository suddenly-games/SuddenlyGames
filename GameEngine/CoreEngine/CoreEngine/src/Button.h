#pragma once

#include "Frame.h"

class Button : public Frame
{
public:
	AppearanceData HoverAppearance;
	AppearanceData ClickAppearance;

	const AppearanceData* GetCurrentAppearance() const;
};
