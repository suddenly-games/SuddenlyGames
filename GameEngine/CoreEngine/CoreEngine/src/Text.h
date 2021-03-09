#pragma once

#include "Object.h"
#include "DeviceVector.h"
#include "RGBA.h"
#include "Alignment.h"

namespace GraphicsEngine
{
	class Font;
	class Texture;
	class FrameBuffer;
	class Appearance;
	class ScreenCanvas;
	class DeviceTransform;

	class Text : public Engine::Object
	{
	public:
		RGBA TextColor;
		bool WrapText = true;
		Enum::Alignment AlignX = Enum::Alignment::Minimum;
		Enum::Alignment AlignY = Enum::Alignment::Minimum;
		DeviceAxis LineSpacing = DeviceAxis(0, 4);
		DeviceAxis FontSize = DeviceAxis(0, 25);
		std::weak_ptr<Font> FontData;

		void Initialize();
		void Update(float) {}

		void Draw(const std::shared_ptr<FrameBuffer> output = nullptr);

		void SetText(const std::string& text);
		std::string GetText() const;

		const std::shared_ptr<Texture>& GetTexture() const;
		const std::shared_ptr<FrameBuffer>& GetBuffer() const;
		const std::shared_ptr<Appearance>& GetAppearance() const;

		static std::shared_ptr<Text> Create(const std::shared_ptr<Font>& font, const std::shared_ptr<ScreenCanvas>& parent, const std::string& name, const std::string& text);

	private:
		bool TextChanged = false;
		float LastFontSize = 0;
		float LastLineSpacing = 0;
		std::string Contents;
		Vector3 LastAbsoluteSize;
		Enum::Alignment LastAlignX = Enum::Alignment::Minimum;
		Enum::Alignment LastAlignY = Enum::Alignment::Minimum;

		std::shared_ptr<Texture> ContentsTexture;
		std::shared_ptr<FrameBuffer> ContentsBuffer;
		std::shared_ptr<Appearance> ContentsAppearance;
		std::shared_ptr<DeviceTransform> TextTransform;
		std::shared_ptr<DeviceTransform> CharacterTransform;

		int GetWordEnd(int start) const;
		bool IsWhiteSpace(int index) const;
		int SkipWhiteSpace(int index) const;
		float GetWordWidth(const std::shared_ptr<Font>& font, int start, int wordEnd) const;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Text);
	};
}