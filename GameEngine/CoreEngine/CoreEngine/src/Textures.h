#pragma once

#include <vector>
#include <map>
#include <glew.h>

#include "Object.h"

namespace GraphicsEngine
{
	class Texture;

	class Textures : public Engine::Object
	{
	public:
		void Initialize() {}
		void Update(float) {}
		~Textures() {}

		static std::shared_ptr<Texture> Create(int width, int height, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool invertedY = false);
		static std::shared_ptr<Texture> Create(const std::string& fileName, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA);
		static std::shared_ptr<Texture> Create(int width, int height, unsigned char* pixelData, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool invertedY = false);

		void Add(const std::shared_ptr<Texture>& texture, const std::string& name);

		Instantiable;

		Inherits_Class(Object);

		Reflected(Textures);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Textures, Engine::Object);
}
