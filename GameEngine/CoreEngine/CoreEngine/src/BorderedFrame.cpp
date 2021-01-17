#include "BorderedFrame.h"

#include "Graphics.h"

BorderedFrame::BorderedFrame(FrameBase* parent) : TopLeftSize(0.1f, 0, 0.1f, 0), BottomRightSize(0.1f, 0, 0.1f, 0)
{
	SetParent(parent);

	Background.SetParent(this);

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			Frames[x][y].SetParent(this);

	Frames[0][0].Data = &TopLeft;
	Frames[0][0].SetAlignment(Enum::Alignment::Minimum, Enum::Alignment::Minimum);
	Frames[0][1].Data = &Top;
	Frames[0][1].SetAlignment(Enum::Alignment::Center, Enum::Alignment::Minimum);
	Frames[0][2].Data = &TopRight;
	Frames[0][2].SetAlignment(Enum::Alignment::Maximum, Enum::Alignment::Minimum);
	Frames[1][0].Data = &Left;
	Frames[1][0].SetAlignment(Enum::Alignment::Minimum, Enum::Alignment::Center);
	Frames[1][1].Data = &Middle;
	Frames[1][1].SetAlignment(Enum::Alignment::Center, Enum::Alignment::Center);
	Frames[1][2].Data = &Right;
	Frames[1][2].SetAlignment(Enum::Alignment::Maximum, Enum::Alignment::Center);
	Frames[2][0].Data = &BottomLeft;
	Frames[2][0].SetAlignment(Enum::Alignment::Minimum, Enum::Alignment::Maximum);
	Frames[2][1].Data = &Bottom;
	Frames[2][1].SetAlignment(Enum::Alignment::Center, Enum::Alignment::Maximum);
	Frames[2][2].Data = &BottomRight;
	Frames[2][2].SetAlignment(Enum::Alignment::Maximum, Enum::Alignment::Maximum);

	SetBorderSize(Vector3(10, 10));
}

void BorderedFrame::SetMode(BorderMode newMode)
{
	Mode = newMode;

	UpdateFrame();
}

void BorderedFrame::SetBorderAspectRatioLocked(bool locked)
{
	BorderAspectRatioLocked = locked;
}

void BorderedFrame::SetBorderSize(const DeviceVector& size)
{
	TopLeftSize = size;
	BottomRightSize = size;

	UpdateFrame();
}

void BorderedFrame::SetBorderSize(const Vector3& size)
{
	TopLeftAspectRatio = size.X / size.Y;
	BottomRightAspectRatio = TopLeftAspectRatio;

	SetBorderSize(DeviceVector(0, size.X, 0, size.Y));
}
void BorderedFrame::SetBorderAspectRatio(float aspectRatio)
{
	BorderAspectRatioLocked = true;

	TopLeftAspectRatio = aspectRatio;
	BottomRightAspectRatio = aspectRatio;

	UpdateFrame();
}

void BorderedFrame::SetTopLeftSize(const DeviceVector& size)
{
	TopLeftSize = size;

	UpdateFrame();
}

void BorderedFrame::SetTopLeftSize(const Vector3& size)
{
	TopLeftAspectRatio = size.X / size.Y;

	SetTopLeftSize(DeviceVector(0, size.X, 0, size.Y));
}

void BorderedFrame::SetTopLeftAspectRatio(float aspectRatio)
{
	BorderAspectRatioLocked = true;

	TopLeftAspectRatio = aspectRatio;

	UpdateFrame();
}

void BorderedFrame::SetBottomRightSize(const DeviceVector& size)
{
	BottomRightSize = size;

	UpdateFrame();
}

void BorderedFrame::SetBottomRightSize(const Vector3& size)
{
	BottomRightAspectRatio = size.X / size.Y;

	SetBottomRightSize(DeviceVector(0, size.X, 0, size.Y));
}

void BorderedFrame::SetBottomRightAspectRatio(float aspectRatio)
{
	BorderAspectRatioLocked = true;

	BottomRightAspectRatio = aspectRatio;

	UpdateFrame();
}

const DeviceVector& BorderedFrame::GetTopLeftSize() const
{
	return TopLeftSize;
}

const DeviceVector& BorderedFrame::GetBottomRightSize() const
{
	return BottomRightSize;
}

float BorderedFrame::GetTopLeftAspectRatio() const
{
	return TopLeftAspectRatio;
}

float BorderedFrame::GetBottomRightAspectRatio() const
{
	return BottomRightAspectRatio;
}

bool BorderedFrame::GetBorderAspectRatioLocked() const
{
	return BorderAspectRatioLocked;
}

Enum::BorderMode BorderedFrame::GetMode() const
{
	return Mode;
}

void BorderedFrame::DrawFrame() const
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			Programs::Screen->blendTexture.Set(Frames[x][y].Data->BlendTexture);
			Programs::Screen->color.Set(Frames[x][y].Data->Color);
			Programs::Screen->textureColor.Set(Frames[x][y].Data->TextureColor);
			Programs::Screen->uvScale.Set(Frames[x][y].Data->UVScale);
			Programs::Screen->uvOffset.Set(Frames[x][y].Data->UVOffset);
			Programs::Screen->transform.Set(Frames[x][y].GetTransformation());
			Programs::Screen->SetTexture(Frames[x][y].Data->Texture);
			Programs::Screen->SetClippingMask(GetClippingMask(), 1);

			Programs::Screen->CoreMeshes.Square->Draw();
		}
	}
}

FrameBase* BorderedFrame::GetCenterFrame()
{
	return &Frames[1][1];
}

void BorderedFrame::UpdateFrame()
{
	bool external = Mode == BorderMode::External && GetParent() != nullptr;

	Vector3 size = external ? GetParent()->GetAbsoluteSize() : GetAbsoluteSize();
	Vector3 topLeft = TopLeftSize.Calculate(Vector3(), size);
	Vector3 bottomRight = BottomRightSize.Calculate(Vector3(), size);

	if (BorderAspectRatioLocked)
	{
		ClampSize(topLeft.X, topLeft.Y, TopLeftAspectRatio);
		ClampSize(bottomRight.X, bottomRight.Y, BottomRightAspectRatio);
	}

	Vector3 topLeftOffset = Vector3(external ? -topLeft.X : 0, external ? -topLeft.Y : 0);
	Vector3 bottomRightOffset = Vector3(external ? bottomRight.X : 0, external ? bottomRight.Y : 0);

	Frames[0][0].SetSizeAndPosition(DeviceVector(0, topLeft.X, 0, topLeft.Y), DeviceVector(0, topLeftOffset.X, 0, topLeftOffset.Y));
	Frames[0][2].SetSizeAndPosition(DeviceVector(0, bottomRight.X, 0, topLeft.Y), DeviceVector(0, bottomRightOffset.X, 0, topLeftOffset.Y));
	Frames[2][0].SetSizeAndPosition(DeviceVector(0, topLeft.X, 0, bottomRight.Y), DeviceVector(0, topLeftOffset.X, 0, bottomRightOffset.Y));
	Frames[2][2].SetSizeAndPosition(DeviceVector(0, bottomRight.X, 0, bottomRight.Y), DeviceVector(0, bottomRightOffset.X, 0, bottomRightOffset.Y));

	Frames[0][1].SetSizeAndPosition(DeviceVector(1, external ? 0 : -topLeft.X - bottomRight.X, 0, topLeft.Y), DeviceVector(0, 0, 0, topLeftOffset.Y));
	Frames[1][0].SetSizeAndPosition(DeviceVector(0, topLeft.X, 1, external ? 0 : -topLeft.Y - bottomRight.Y), DeviceVector(0, topLeftOffset.X, 0, 0));
	Frames[1][2].SetSizeAndPosition(DeviceVector(0, bottomRight.X, 1, external ? 0 : -topLeft.Y - bottomRight.Y), DeviceVector(0, bottomRightOffset.X, 0, 0));
	Frames[2][1].SetSizeAndPosition(DeviceVector(1, external ? 0 : -topLeft.X - bottomRight.X, 0, bottomRight.Y), DeviceVector(0, 0, 0, bottomRightOffset.Y));

	Frames[1][1].SetSize(DeviceVector(1, external ? 0 : -topLeft.X - bottomRight.X, 1, external ? 0 : -topLeft.Y - bottomRight.Y));

	if (Frames[1][1].GetParent() != this)
		Frames[1][1].SetParent(this);
}
