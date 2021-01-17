#pragma once

#include <string>
#include "SOIL.h"
extern "C" {
#include "glew.h"
}

#include "Vector3.h"
#include "RGBA.h"
#include "Object.h"
#include "Dimensions.h"

std::ostream& operator<<(std::ostream& out, const Dimensions& dimensions);

namespace GraphicsEngine
{
	class Texture : public Engine::Object
	{
	public:
		struct SampleTypeEnum
		{
			enum SampleType
			{
				Nearest = GL_NEAREST,
				Linear = GL_LINEAR
			};
		};

		struct WrapTypeEnum
		{
			enum WrapType
			{ 
				Repeat = GL_REPEAT,
				ClampCutOff = GL_CLAMP_TO_BORDER,
				ClampExtend = GL_CLAMP_TO_EDGE
			};
		};

		struct InternalFormatEnum
		{
			enum InternalFormat
			{
				RGBA = GL_RGBA
			};
		};

		struct FormatEnum
		{
			enum Format
			{
				RGBA = GL_RGBA
			};
		};

		struct DataTypeEnum
		{
			enum DataType
			{
				UnsignedByte = GL_UNSIGNED_BYTE,
				Float = GL_FLOAT
			};
		};

		void Initialize() {}
		void Update(float) {}

		~Texture();

		void Resize(int width, int height);
		void Resize(Dimensions size);
		void Load(int width, int height, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool invertedY = false);
		void Load(const std::string& fileName, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool isDXT5 = false);
		void Load(int width, int height, unsigned char* pixelData, GLint sampleType = GL_NEAREST, GLint wrapType = GL_REPEAT, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool invertedY = false);
		GLuint GetTextureID() const;
		int GetWidth() const;
		int GetHeight() const;
		Vector3 GetResolution() const;
		unsigned char* GetPixelData();
		RGBA GetPixel(int x, int y) const;
		bool HasInvertedY() const;

		void LoadPiece(const std::string& fileName, int x, int y, int width, int height, GLenum dataType = GL_UNSIGNED_BYTE, GLenum format = GL_RGBA);

	private:
		GLint SampleType = GL_NEAREST;
		GLint WrapType = GL_REPEAT;
		GLenum DataType = GL_UNSIGNED_BYTE;
		GLint InternalFormat = GL_RGBA;
		GLenum Format = GL_RGBA;

		bool IsDXT5nm = false;
		bool InvertedY = false;
		GLuint TextureID;
		Dimensions Size;
		unsigned char* PixelData = nullptr;
		bool SoilTexture = false;

		Instantiable;

		Inherits_Class(Object);

		Reflected(Texture);
	};
}

namespace Enum
{
	typedef GraphicsEngine::Texture::SampleTypeEnum::SampleType SampleType;
	typedef GraphicsEngine::Texture::WrapTypeEnum::WrapType WrapType;
	typedef GraphicsEngine::Texture::InternalFormatEnum::InternalFormat InternalFormat;
	typedef GraphicsEngine::Texture::FormatEnum::Format Format;
	typedef GraphicsEngine::Texture::DataTypeEnum::DataType DataType;
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::Texture, Object);
	//Class_Inherits(Dimensions, CoreType);

	Declare_Enum(SampleType);
	Declare_Enum(WrapType);
	Declare_Enum(InternalFormat);
	Declare_Enum(Format);
	Declare_Enum(DataType);
}
