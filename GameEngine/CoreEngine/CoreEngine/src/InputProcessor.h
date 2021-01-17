#pragma once

#include "Graphics.h"
#include "FrameBase.h"

class InputProcessor
{
public:
	FrameBase* Focus = nullptr;
	FrameBase* LeftClickedFrame = nullptr;
	FrameBase* MiddleClickedFrame = nullptr;
	FrameBase* RightClickedFrame = nullptr;

	InputProcessor();

	void Process(FrameBase* frame);
};
