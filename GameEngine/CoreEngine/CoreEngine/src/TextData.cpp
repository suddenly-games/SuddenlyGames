#include "TextData.h"
#include "FrameBase.h"

void TextData::Draw(const Vector3& position) const
{
	Draw(position, Graphics::ActiveWindow->Resolution);
}

void TextData::Draw(const Vector3& position, const Vector3& resolution) const
{
	Programs::Screen->color.Set(0xFFFFFF00);
	Programs::Screen->textureColor.Set(Color);
	Programs::Screen->blendTexture.Set(true);
	Programs::Screen->SetTexture(Handle<GraphicsEngine::Font>(FontType)->GetTexture());

	for (int i = 0; i < int(RawTextData.size()); ++i)
	{
		const CharacterData& character = RawTextData[i];

		Programs::Screen->uvScale.Set(character.UVScale);
		Programs::Screen->uvOffset.Set(character.UVOffset);

		//if (transformationSet)
		//	Programs::Screen->transform.Set(character.Transformation);
		//else
		//	Programs::Screen->transform.Set(FrameBase::CalculateScreenTransformation(character.size(), position + character.Position, resolution));

		//mesh->Draw();
	}
}

void TextData::Draw(const Vector3& position, const FrameBase* frame) const
{
	Programs::Screen->color.Set(0xFFFFFF00);
	Programs::Screen->textureColor.Set(Color);
	Programs::Screen->blendTexture.Set(true);
	Programs::Screen->SetTexture(Handle<GraphicsEngine::Font>(FontType)->GetTexture());

	for (int i = 0; i < int(RawTextData.size()); ++i)
	{
		const CharacterData& character = RawTextData[i];

		Programs::Screen->uvScale.Set(character.UVScale);
		Programs::Screen->uvOffset.Set(character.UVOffset);

		//if (transformationSet)
		//	Programs::Screen->transform.Set(character.Transformation);
		//else
			Programs::Screen->transform.Set(frame->GetTransformation() * frame->CalculateLocalTransformation(RawTextData[i].Size, position + RawTextData[i].Position));
		//
		Programs::Screen->CoreMeshes.Square->Draw();
	}
}

void TextData::SetText(const std::string& newText)
{
	Text = newText;

	UpdateTextData();
}

void TextData::SetFontSize(float size)
{
	FontSize = size;

	UpdateTextData();
}

void TextData::SetMaxBounds(const Vector3& bounds)
{
	MaxBounds = bounds;

	UpdateTextData();
}

void TextData::SetFont(Handle<GraphicsEngine::Font> newFont)
{
	FontType = newFont;

	UpdateTextData();
}

const std::string& TextData::GetText() const
{
	return Text;
}

float TextData::GetFontSize() const
{
	return FontSize;
}

const Vector3& TextData::GetTextBounds() const
{
	return TextBounds;
}

const Vector3& TextData::GetMaxBounds() const
{
	return MaxBounds;
}

Handle<GraphicsEngine::Font> TextData::GetFont() const
{
	return FontType;
}

void TextData::UpdateTransformation(const Vector3& position, const FrameBase* frame)
{
	TransformationSet = true;

	for (int i = 0; i < int(Text.size()); ++i)
		RawTextData[i].Transformation = frame->GetTransformation() * frame->CalculateLocalTransformation(RawTextData[i].Size, position + RawTextData[i].Position);
}

void TextData::UpdateTextData()
{
	RawTextData.resize(Text.size());
	TextBounds.Set(0, 0);

	Vector3 characterPosition;

	int characterIndex = 0;

	for (int i = 0; i < int(Text.size()); ++i)
	{
		char character = Text[i];

		switch (character)
		{
		case ' ':
			characterPosition.X += FontType->SpaceWidth * FontSize;

			break;
		case '\t':
			characterPosition.X += FontType->SpaceWidth * FontType->TabSpaces * FontSize;

			break;
		case '\n':
		case '\r':
			if (characterPosition.X > TextBounds.X)
				TextBounds.X = characterPosition.X;

			characterPosition.X = 0;
			characterPosition.Y += FontSize;

			break;
		default:
			CharacterData& characterSettings = RawTextData[characterIndex];

			const GraphicsEngine::Font::Character& characterData = FontType->GetCharacter(character);

			float width = FontType->GetTexture()->GetWidth() * characterData.TextScale.X;
			float height = FontType->GetTexture()->GetHeight() * characterData.TextScale.Y;
			float aspectRatio = width / height;

			characterSettings.UVScale = Vector3(characterData.TextScale.X, -characterData.TextScale.Y);
			characterSettings.UVOffset = Vector3(characterData.TextOffset.X, characterData.TextOffset.Y + characterData.TextScale.Y);
			characterSettings.Position = characterPosition;
			characterSettings.Size = Vector3(aspectRatio * FontSize, FontSize);

			characterPosition.X += aspectRatio * FontSize;

			++characterIndex;
		}
	}

	RawTextData.resize(characterIndex);

	if (characterPosition.X > TextBounds.X)
		TextBounds.X = characterPosition.X;

	TextBounds.Y = characterPosition.Y + FontSize;
}
