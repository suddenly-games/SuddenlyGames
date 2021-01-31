#include "Font.h"

#include <fstream>

#include "Textures.h"
#include "Texture.h"

namespace GraphicsEngine
{
	Font::Character::Character() : Value(0), TextOffset(), TextScale() {}

	Font::Character::Character(char character, float aspectRatio, const Vector3& textOffset, const Vector3& textScale) : Value(character), AspectRatio(aspectRatio), TextOffset(textOffset), TextScale(textScale) {}


	void Font::Load(const std::string& filePath, const std::string& textureName)
	{
		std::ifstream file;
		file.open(filePath);

		if (!file.is_open())
			return;

		std::string texturePath;

		std::getline(file, texturePath);

		std::shared_ptr<Texture> texture = Textures::Create(texturePath);

		texture->Name = textureName;
		texture->SetParent(This.lock());

		// readd find texture

		Glyphs = texture;

		Vector3 resolution;

		file >> resolution.X >> resolution.Y;

		while (!file.eof())
		{
			Vector3 scale, offset;
			char character;

			file >> character >> scale.X >> scale.Y >> offset.X >> offset.Y;

			float aspectRatio = scale.X / scale.Y;

			scale.X /= resolution.X;
			scale.Y /= resolution.Y;
			offset.X /= resolution.X;
			offset.Y /= resolution.Y;

			Characters[character] = Character(character, aspectRatio, offset, scale);
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
