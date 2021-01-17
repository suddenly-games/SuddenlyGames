#pragma once

#include <string>
#include <map>

#include "Vector3.h"
#include "Object.h"

namespace GraphicsEngine
{
	class Texture;

	class Font : public Engine::Object
	{
	public:
		class Character
		{
		public:
			char Value;
			Vector3 TextOffset;
			Vector3 TextScale;

			Character();
			Character(char character, const Vector3& textOffset, const Vector3& textScale);

			Base_Class;

			Reflected_Type(Character);
		};

		void Initialize() {}
		void Update(float) {}

		float SpaceWidth = 0.3f;
		int TabSpaces = 4;

		void Load(const std::string& filePath, const std::string& textureName);
		const Character& GetCharacter(char character) const;
		std::shared_ptr<Texture> GetTexture() const;

	private:
		typedef std::map< char, Character > CharacterMap;

		CharacterMap Characters;
		std::weak_ptr<Texture> Glyphs;

		Character Nope = Character(0, Vector3(), Vector3());

		Instantiable;

		Inherits_Class(Object);

		Reflected(Font);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Font, Engine::Object);
	//Class_Inherits(GraphicsEngine::Font::Character, CoreClass);

	namespace
	{
		typedef GraphicsEngine::Font::Character Character;
	}

	Define_Value_Type(Character);
}
