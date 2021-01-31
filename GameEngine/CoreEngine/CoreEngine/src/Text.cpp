#include "Text.h"

#include "Font.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Appearance.h"
#include "ScreenCanvas.h"
#include "DeviceTransform.h"
#include "Graphics.h"

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

		CharacterTransform = Engine::Create<DeviceTransform>();
		CharacterTransform->Name = "CharacterTransform";
		CharacterTransform->SetParent(This.lock());
	}

	void Text::Draw(const std::shared_ptr<FrameBuffer> output)
	{
		std::shared_ptr<DeviceTransform> transform = GetComponent<DeviceTransform>();

		if (transform == nullptr)
			return;

		std::shared_ptr<Font> font = FontData.lock();

		if (font == nullptr)
			return;

		Vector3 absoluteSize = transform->GetAbsoluteSize();
		float fontSize = FontSize.Calculate(0, absoluteSize.Y);
		float lineSpacing = LineSpacing.Calculate(0, absoluteSize.Y);

		if (!TextChanged && fontSize == LastFontSize && absoluteSize == LastAbsoluteSize)// && AlignX == LastAlignX && AlignY == LastAlignY)
			return;

		if (LastAbsoluteSize != absoluteSize)
			ContentsBuffer->Resize(Dimensions(absoluteSize));

		TextChanged = false;
		LastFontSize = fontSize;
		LastAbsoluteSize = absoluteSize;

		Programs::Screen->Use();

		Programs::Screen->SetClippingMask(nullptr, 1);
		Programs::Screen->SetTexture(font->GetTexture());
		Programs::Screen->color.Set(RGBA(0, 0, 0, 0));
		Programs::Screen->blendTexture.Set(true);
		Programs::Screen->resolution.Set(absoluteSize);
		Programs::Screen->textureColor.Set(TextColor);

		ContentsBuffer->DrawTo();

		CharacterTransform->Position = DeviceVector(0, 0, 0, 0);

		for (int i = SkipWhiteSpace(0); i < int(Contents.size()); i = SkipWhiteSpace(i))
		{
			int wordEnd = GetWordEnd(i);
			float wordWidth = GetWordWidth(font, i, wordEnd);

			if (WrapText && CharacterTransform->Position.X.Offset + wordWidth > absoluteSize.X)
			{
				CharacterTransform->Position.X.Offset = 0;
				CharacterTransform->Position.Y.Offset += fontSize + lineSpacing;
			}

			const Font::Character& data = font->GetCharacter(Contents[i]);

			CharacterTransform->Size = DeviceVector(0, data.AspectRatio * fontSize, 0, fontSize);
			CharacterTransform->UpdateTransformation();

			Programs::Screen->uvScale.Set(data.TextScale);
			Programs::Screen->uvOffset.Set(data.TextOffset);
			Programs::Screen->transform.Set(CharacterTransform->GetTransformation());

			Programs::Screen->CoreMeshes.Square->Draw();

			CharacterTransform->Position.X.Offset += CharacterTransform->Size.X.Offset;

			i = wordEnd;
		}

		if (output != nullptr)
			output->DrawTo();
		else
			FrameBuffer::Detatch();
	}

	void Text::SetText(const std::string& text)
	{
		TextChanged = true;

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

	int Text::GetWordEnd(int start) const
	{
		for (start; start < int(Contents.size()) && !IsWhiteSpace(start); ++start);

		return start;
	}

	bool Text::IsWhiteSpace(int index) const
	{
		return Contents[index] == ' ' || Contents[index] == '\t' || Contents[index] == '\n' || Contents[index] == '\r';
	}

	int Text::SkipWhiteSpace(int index) const
	{
		for (index; index < int(Contents.size()) && IsWhiteSpace(index); ++index);

		return index;
	}

	float Text::GetWordWidth(const std::shared_ptr<Font>& font, int start, int wordEnd) const
	{
		float width = 0;

		for (int i = start; i < wordEnd; ++i)
		{
			const Font::Character& character = font->GetCharacter(Contents[i]);

			width += character.AspectRatio * LastFontSize;
		}

		return width;
	}
}