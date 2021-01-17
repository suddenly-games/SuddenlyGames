#include "ScrollFrame.h"

ScrollFrame::ScrollFrame()
{
	Border.SetParent(this);
	Background.SetParent(&Border);
	Canvas.SetParent(&Border);
	HorizontalBar.SetParent(this);
	VerticalBar.SetParent(this);

	HorizontalBar.SetAlignmentY(Enum::Alignment::Maximum);
	VerticalBar.SetAlignmentX(Enum::Alignment::Maximum);
	HorizontalBar.SetDirection(Enum::SliderDirection::Horizontal);
	VerticalBar.SetDirection(Enum::SliderDirection::Vertical);

	HorizontalBar.ScrollSlider.SliderMoved.Connect([this](float) -> bool
	{
		Vector3 totalSize = Canvas.GetAbsoluteSize() - Border.GetAbsoluteSize();

		Canvas.SetPosition(DeviceVector(0, -totalSize.X * HorizontalBar.ScrollSlider.GetPercentage(), 0, -totalSize.Y * VerticalBar.ScrollSlider.GetPercentage()));

		return true;
	});

	VerticalBar.ScrollSlider.SliderMoved.Connect([this](float) -> bool
	{
		Vector3 totalSize = Canvas.GetAbsoluteSize() - Border.GetAbsoluteSize();

		Canvas.SetPosition(DeviceVector(0, -totalSize.X * HorizontalBar.ScrollSlider.GetPercentage(), 0, -totalSize.Y * VerticalBar.ScrollSlider.GetPercentage()));

		return true;
	});

	Canvas.Mouse.Wheel.Moved.Connect([this](InputObject* object) -> bool
	{
		Vector3 delta = object->GetDelta();

		HorizontalBar.ScrollSlider.Scrolled(delta.X);
		VerticalBar.ScrollSlider.Scrolled(-delta.Y);

		return true;
	});
}

void ScrollFrame::SetScrollBarWidth(DeviceAxis width)
{
	BarWidth = width;

	UpdateFrame();
}

void ScrollFrame::SetScrollBarWidth(float scale, float offset)
{
	BarWidth.Set(scale, offset);

	UpdateFrame();
}

void ScrollFrame::SetBarVisibility(bool horizontal, bool vertical)
{
	HorizontalBar.Enabled = horizontal;
	VerticalBar.Enabled = vertical;

	UpdateFrame();
}

void ScrollFrame::SetCanvasSize(DeviceVector size)
{
	CanvasSize = size;

	UpdateFrame();
}

DeviceAxis ScrollFrame::GetScrollBarWidth() const
{
	return BarWidth;
}

const DeviceVector& ScrollFrame::GetCanvasSize() const
{
	return CanvasSize;
}

void ScrollFrame::UpdateFrame()
{
	DeviceAxis max(1, 0);
	DeviceAxis min(0, 0);
	DeviceAxis width = max - (VerticalBar.Enabled ? BarWidth : min);
	DeviceAxis height = max - (HorizontalBar.Enabled ? BarWidth : min);

	HorizontalBar.SetSize(DeviceVector(width, BarWidth));
	VerticalBar.SetSize(DeviceVector(BarWidth, height));
	Border.SetSize(DeviceVector(width, height));

	Vector3 size = GetAbsoluteSize();
	Vector3 canvasTotalSize = CanvasSize.Calculate(Vector3(), size);
	Vector3 borderSize = Border.GetAbsoluteSize();

	canvasTotalSize.X *= borderSize.X == 0 ? 0 : size.X / borderSize.X;
	canvasTotalSize.Y *= borderSize.Y == 0 ? 0 : size.Y / borderSize.Y;

	Canvas.SetSize(DeviceVector(0, canvasTotalSize.X, 0, canvasTotalSize.Y));

	float barSize = canvasTotalSize.X == 0 ? 1 : borderSize.X / canvasTotalSize.X;

	HorizontalBar.ScrollSlider.SetSliderSize(barSize > 1 ? 1 : barSize);

	barSize = canvasTotalSize.Y == 0 ? 1 : borderSize.Y / canvasTotalSize.Y;

	VerticalBar.ScrollSlider.SetSliderSize(barSize > 1 ? 1 : barSize);
}
