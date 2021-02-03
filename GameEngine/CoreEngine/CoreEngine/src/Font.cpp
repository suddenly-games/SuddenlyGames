#include "Font.h"

#include <fstream>

#include "Textures.h"
#include "Texture.h"

namespace GraphicsEngine
{
	Font::Character::Character() : Value(0), TextOffset(), TextScale() {}

	Font::Character::Character(char character, float aspectRatio, const Vector3& textOffset, const Vector3& textScale, float kerningLeft, float kerningRight, float verticalOffset) : Value(character), AspectRatio(aspectRatio), TextOffset(textOffset), TextScale(textScale), KerningLeft(kerningLeft), KerningRight(kerningRight), VerticalOffset(verticalOffset) {}


	void Font::Load(const std::string& filePath, const std::string& textureName)
	{
		std::ifstream file;
		file.open(filePath);

		if (!file.is_open())
			return;

		std::string texturePath;

		std::getline(file, texturePath);

		std::shared_ptr<Texture> texture = Textures::Create(texturePath, Enum::SampleType::Linear);

		texture->Name = textureName;
		texture->SetParent(This.lock());

		// readd find texture

		Glyphs = texture;

		Vector3 resolution;
		std::string flipTextureFlag;
		std::string flipCharacterFlag;

		file >> resolution.X >> resolution.Y >> flipTextureFlag >> flipCharacterFlag;

		bool flipY = flipTextureFlag == "true";
		bool flipCharacterY = flipCharacterFlag == "true";

		while (!file.eof())
		{
			Vector3 scale, offset;
			char character;
			float kerningLeft = 0;
			float kerningRight = 0;
			float verticalOffset = 0;

			file >> character >> scale.X >> scale.Y >> offset.X >> offset.Y >> kerningLeft >> kerningRight >> verticalOffset;

			float aspectRatio = scale.X / scale.Y;

			kerningLeft /= scale.Y;
			kerningRight /= scale.Y;
			verticalOffset /= scale.Y;

			scale.X /= resolution.X;
			scale.Y /= resolution.Y;
			offset.X /= resolution.X;
			offset.Y /= resolution.Y;

			if (flipY)
			{
				scale.Y *= -1;
				offset.Y = 1 - offset.Y;
			}

			if (flipCharacterY)
			{
				offset.Y += scale.Y;
				scale.Y *= -1;
			}

			Characters[character] = Character(character, aspectRatio, offset, scale, kerningLeft, kerningRight, verticalOffset);
		}

		file.close();
	}

	const Font::Character& Font::GetCharacter(char character) const
	{
		CharacterMap::const_iterator i = Characters.find(character);

		if (i != Characters.end())
			return i->second;

		return Nope;
	}

	std::shared_ptr<Texture> Font::GetTexture() const
	{
		return Glyphs.lock();
	}
}
