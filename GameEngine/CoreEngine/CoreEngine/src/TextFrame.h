#pragma once

#include "Frame.h"
#include "TextData.h"

class TextFrame : public FrameBase
{
public:
	TextData Text;
	DeviceAxis FontSize;
	Alignment TextAlignmentX = Alignment::Center;
	Alignment TextAlignmentY = Alignment::Center;

	TextFrame();

	void DrawFrame() const;
	void UpdateFrame();
};
