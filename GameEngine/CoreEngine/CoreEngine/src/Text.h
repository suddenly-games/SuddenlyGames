#pragma once

#include "Object.h"
#include "DeviceVector.h"

namespace GraphicsEngine
{
	class Font;
	class Texture;
	class FrameBuffer;
	class Appearance;
	class ScreenCanvas;

	class Text : public Engine::Object
	{
	public:
		DeviceAxis FontSize;
		std::weak_ptr<Font> FontData;

		void Initialize();
		void Update(float) {}

		void Draw();

		void SetText(const std::string& text);
		std::string GetText() const;

		const std::shared_ptr<Texture>& GetTexture() const;
		const std::shared_ptr<FrameBuffer>& GetBuffer() const;
		const std::shared_ptr<Appearance>& GetAppearance() const;

		static std::shared_ptr<Text> Create(const std::shared_ptr<Font>& font, const std::shared_ptr<ScreenCanvas>& parent, const std::string& name, const std::string& text);

	private:
		float LastFontSize = 0;
		std::string Contents;
		Vector3 LastAbsoluteSize;

		std::shared_ptr<Texture> ContentsTexture;
		std::shared_ptr<FrameBuffer> ContentsBuffer;
		std::shared_ptr<Appearance> ContentsAppearance;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Text);
	};
}