#include "Text.h"

#include "Texture.h"
#include "FrameBuffer.h"
#include "Appearance.h"
#include "ScreenCanvas.h"

namespace GraphicsEngine
{
	void Text::Initialize()
	{
		ContentsTexture = Engine::Create<Texture>();
		ContentsTexture->Name = "ContentsTexture";

		ContentsBuffer = FrameBuffer::Create(1, 1, ContentsTexture, true, false);
		ContentsBuffer->Name = "ContentsBuffer";
		ContentsBuffer->SetParent(This.lock());

		ContentsAppearance = Engine::Create<Appearance>();
		ContentsAppearance->Name = "ContentsAppearance";
		ContentsAppearance->Texture = ContentsTexture;
		ContentsAppearance->SetParent(This.lock());
	}

	void Text::Draw()
	{

	}

	void Text::SetText(const std::string& text)
	{
		Contents = text;
	}

	std::string Text::GetText() const
	{
		return Contents;
	}

	const std::shared_ptr<Texture>& Text::GetTexture() const
	{
		return ContentsTexture;
	}

	const std::shared_ptr<FrameBuffer>& Text::GetBuffer() const
	{
		return ContentsBuffer;
	}

	const std::shared_ptr<Appearance>& Text::GetAppearance() const
	{
		return ContentsAppearance;
	}

	std::shared_ptr<Text> Text::Create(const std::shared_ptr<Font>& font, const std::shared_ptr<ScreenCanvas>& parent, const std::string& name, const std::string& text)
	{
		std::shared_ptr<Text> textObject = Engine::Create<Text>();

		textObject->Name = name;
		textObject->FontData = font;
		textObject->SetText(text);
		textObject->SetParent(parent);

		parent->Appearance = textObject->ContentsAppearance;

		return textObject;
	}
}