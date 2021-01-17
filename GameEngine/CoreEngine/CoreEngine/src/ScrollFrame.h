#pragma once

#include "ScrollBar.h"

class ScrollFrame : public FrameBase
{
public:
	ScrollBar HorizontalBar;
	ScrollBar VerticalBar;
	FrameBase Background;
	FrameBase Border;
	FrameBase Canvas;

	ScrollFrame();

	void SetScrollBarWidth(DeviceAxis width);
	void SetScrollBarWidth(float scale, float offset);
	void SetBarVisibility(bool horizontal, bool vertical);
	void SetCanvasSize(DeviceVector size);
	DeviceAxis GetScrollBarWidth() const;
	const DeviceVector& GetCanvasSize() const;
	void UpdateFrame();

private:
	DeviceAxis BarWidth = DeviceAxis(0.1f, 0);
	DeviceVector CanvasSize = DeviceVector(1, 0, 1, 0);
};
