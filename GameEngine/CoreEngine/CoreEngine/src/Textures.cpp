#include "Textures.h"

#include "Texture.h"
#include <filesystem>

namespace GraphicsEngine
{
	std::shared_ptr<Texture> Textures::Create(int width, int height, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format, bool invertedY)
	{
		std::shared_ptr<Texture> texture = Engine::Create<Texture>();

		texture->Load(width, height, sampleType, wrapType, dataType, internalFormat, format, invertedY);

		return texture;
	}

	std::shared_ptr<Texture> Textures::Create(const std::string& fileName, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format)
	{
		std::shared_ptr<Texture> texture = Engine::Create<Texture>();

		texture->Load(fileName, sampleType, wrapType, dataType, internalFormat, format, fileName.size() >= 4 && fileName.substr(fileName.size() - 4, 4) == ".dds");

		return texture;
	}

	std::shared_ptr<Texture> Textures::Create(int width, int height, unsigned char* pixelData, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format, bool invertedY)
	{
		std::shared_ptr<Texture> texture = Engine::Create<Texture>();

		texture->Load(width, height, pixelData, sampleType, wrapType, dataType, internalFormat, format, invertedY);

		return texture;
	}

	void Textures::LoadDirectory(const std::string& folderPath, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format)
	{

		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {

			std::string path = entry.path().string();
			std::string stem = entry.path().stem().string();
			std::string extension = entry.path().extension().string();

			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".dds") {

				std::shared_ptr<Texture> texture = Textures::Create(folderPath + "/" + stem + extension);
				Add(texture, stem);
			}
		}
	}

	void Textures::Add(const std::shared_ptr<Texture>& texture, const std::string& name)
	{
		texture->Name = name;
		texture->SetParent(This.lock());
	}
}
