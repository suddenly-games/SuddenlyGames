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
		ContentsAppearance->Color = RGBA(0, 0, 0, 0);
		//ContentsAppearance->TextureColor = RGBA(0, 0, 0, 0);
		ContentsAppearance->SetParent(This.lock());

		TextTransform = Engine::Create<DeviceTransform>();
		TextTransform->Name = "TextTransform";
		TextTransform->InheritTransformation = false;
		TextTransform->SetParent(This.lock());

		CharacterTransform = Engine::Create<DeviceTransform>();
		CharacterTransform->Name = "CharacterTransform";
		CharacterTransform->SetParent(TextTransform);
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

		if (!TextChanged && fontSize == LastFontSize && absoluteSize == LastAbsoluteSize && AlignX == LastAlignX && AlignY == LastAlignY)
			return;

		TextTransform->Size = DeviceVector(0, absoluteSize.X, 0, absoluteSize.Y);

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

		Graphics::SetClearColor(RGBA(0, 0, 0, 0)); CheckGLErrors();
		Graphics::ClearScreen(GL_COLOR_BUFFER_BIT); CheckGLErrors();
		CharacterTransform->AnchorPoint = DeviceVector(0, 0, 0, 0);
		CharacterTransform->Position = DeviceVector(0, 0, 0, 0);

		Vector3 offsetScale(1, 1);

		if (AlignX == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.X.Scale = 0;
			CharacterTransform->Position.X.Scale = 0;
		}
		else if (AlignX == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.X.Scale = 0.5f;
			CharacterTransform->Position.X.Scale = 0.5f;
		}
		else if (AlignX == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.X.Scale = 1;
			CharacterTransform->Position.X.Scale = 1;
		}

		if (AlignY == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.Y.Scale = 0;
			CharacterTransform->Position.Y.Scale = 0;
		}
		else if (AlignY == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.Y.Scale = 0.5f;
			CharacterTransform->Position.Y.Scale = 0.5f;
		}
		else if (AlignY == Enum::Alignment::Minimum)
		{
			CharacterTransform->AnchorPoint.Y.Scale = 1;
			CharacterTransform->Position.Y.Scale = 1;
		}

		for (int i = SkipWhiteSpace(0); i < int(Contents.size()); i)
		{
			int wordEnd = GetWordEnd(i);
			float wordWidth = GetWordWidth(font, i, wordEnd);

			if (WrapText && CharacterTransform->Position.X.Offset + wordWidth > absoluteSize.X)
			{
				CharacterTransform->Position.X.Offset = 0;
				CharacterTransform->Position.Y.Offset += fontSize + lineSpacing;
			}
			
			for (i; i < wordEnd; ++i)
			{
				const Font::Character& data = font->GetCharacter(Contents[i]);

				CharacterTransform->Position.X.Offset -= data.KerningLeft * fontSize;
				CharacterTransform->Size = DeviceVector(0, data.AspectRatio * fontSize, 0, fontSize);
				CharacterTransform->UpdateTransformation();

				Programs::Screen->uvScale.Set(data.TextScale);
				Programs::Screen->uvOffset.Set(data.TextOffset);
				Programs::Screen->transform.Set(CharacterTransform->GetTransformation());

				Programs::Screen->CoreMeshes.Square->Draw();

				CharacterTransform->Position.X.Offset += CharacterTransform->Size.X.Offset - fontSize * data.KerningRight;
			}

			for (i; i < SkipWhiteSpace(i); ++i)
			{
				if (Contents[i] == ' ')
					CharacterTransform->Position.X.Offset += font->SpaceWidth * fontSize;
				else if (Contents[i] == '\t')
				{
					float tabSize = font->TabSpaces * font->SpaceWidth * fontSize;

					CharacterTransform->Position.X.Offset += (std::floorf(CharacterTransform->Position.X.Offset / tabSize) + 1) * tabSize;
				}
				else if (Contents[i] == '\r' || Contents[i] == '\n')
				{
					CharacterTransform->Position.X.Offset = 0;
					CharacterTransform->Position.Y.Offset += fontSize + lineSpacing;
				}
			}
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

			width += (character.AspectRatio - character.KerningLeft - character.KerningRight) * LastFontSize;
		}

		return width;
	}
}