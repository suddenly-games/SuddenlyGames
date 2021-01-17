#pragma once

#include "RGBA.h"
#include "Matrix3.h"
#include "FrameBase.h"
#include "Texture.h"
#include "Mesh.h"
#include "Frame.h"

class BorderedFrame : public FrameBase
{
public:
	struct BorderModeEnum
	{
		enum BorderMode
		{
			Internal,
			External
		};
	};

	typedef BorderModeEnum::BorderMode BorderMode;

	class BorderFrame : public FrameBase
	{
	public:
		AppearanceData* Data;

		void DrawFrame() const {}
	};

	AppearanceData TopLeft;
	AppearanceData Top;
	AppearanceData TopRight;
	AppearanceData Left;
	AppearanceData Middle;
	AppearanceData Right;
	AppearanceData BottomLeft;
	AppearanceData Bottom;
	AppearanceData BottomRight;

	FrameBase Background;
	BorderFrame Frames[3][3];

	BorderedFrame(FrameBase* parent = nullptr);

	void SetMode(BorderMode newMode);
	void SetBorderAspectRatioLocked(bool locked);
	void SetBorderSize(const DeviceVector& size);
	void SetBorderSize(const Vector3& size);
	void SetBorderAspectRatio(float aspectRatio);
	void SetTopLeftSize(const DeviceVector& size);
	void SetTopLeftSize(const Vector3& size);
	void SetTopLeftAspectRatio(float aspectRatio);
	void SetBottomRightSize(const DeviceVector& size);
	void SetBottomRightSize(const Vector3& size);
	void SetBottomRightAspectRatio(float aspectRatio);
	const DeviceVector& GetTopLeftSize() const;
	const DeviceVector& GetBottomRightSize() const;
	float GetTopLeftAspectRatio() const;
	float GetBottomRightAspectRatio() const;
	bool GetBorderAspectRatioLocked() const;
	BorderMode GetMode() const;
	void DrawFrame() const;
	FrameBase* GetCenterFrame();
	void UpdateFrame();

private:
	bool BorderAspectRatioLocked = false;
	float TopLeftAspectRatio = 1;
	float BottomRightAspectRatio = 1;
	BorderMode Mode = BorderMode::Internal;
	DeviceVector TopLeftSize;
	DeviceVector BottomRightSize;
};

namespace Enum
{
	typedef BorderedFrame::BorderMode BorderMode;
}
