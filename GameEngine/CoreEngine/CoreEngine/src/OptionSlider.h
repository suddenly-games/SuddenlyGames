#pragma once

#include <functional>

#include "FrameBase.h"
#include "Button.h"
#include "TextFrame.h"

class OptionSlider : public FrameBase
{
public:
	struct SliderDirectionEnum
	{
		enum SliderDirection
		{
			Vertical,
			Horizontal
		};
	};

	typedef SliderDirectionEnum::SliderDirection SliderDirection;
	typedef const char* const* StringArray;
	typedef std::function<AppearanceData(const char* text, int position)> OptionAppearanceCallback;

	float SliderSpeed = 5;
	int CurrentOption = 0;

	DeviceAxis FontSize = DeviceAxis(0, 10);
	RGBA FontColor = 0x000000FF;
	Handle<GraphicsEngine::Font> FontType;

	AppearanceData SliderAppearance;
	AppearanceData SliderHoverAppearance;
	AppearanceData SliderClickAppearance;

	AppearanceData OptionAppearance;
	AppearanceData OptionHoverAppearance;
	AppearanceData OptionClickAppearance;

	Event<int> ValueChanged;

	OptionSlider();

	void SetOptions(StringArray optionNames);
	void SetDirection(SliderDirection newDirection);
	void SetOptionSize(const DeviceVector& newOptionSize);
	void SetOptionAspectRatio(float aspectRatio);
	void SetOptionAspectRatioLocked(bool locked);
	void RefreshAppearance();
	void Scrolled(int ticks);
	FrameBase* GetBackgroundContainer();
	std::string GetCurrentOptionName() const;
	std::string GetOptionName(int id) const;
	int GetOptionCount() const;
	Button* GetSlider();
	SliderDirection GetDirection() const;
	void UpdateFrame();

private:
	typedef std::vector<Button*> ButtonVector;
	typedef std::vector<TextFrame*> TextVector;

	float OptionAspectRatio = 1;
	bool LockOptionAspectRatio = false;
	SliderDirection Direction = SliderDirection::Horizontal;
	ButtonVector Options;
	TextVector OptionLabels;
	DeviceVector OptionSize = DeviceVector(0.1f, 0, 0.1f, 0);
	FrameBase BackgroundContainer;
	FrameBase OptionContainer;
	FrameBase OptionLabelContainer;
	Button Slider;
	
	void ClearOptions();
};

namespace Enum
{
	typedef OptionSlider::SliderDirection SliderDirection;
}
