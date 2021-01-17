#include "Texture.h"

#include <fstream>

#include "Graphics.h"
#include "FrameBuffer.h"

Dimensions::operator std::string()
{
	std::stringstream out;

	out << *this;
	
	return out.str();
}

std::ostream& operator<<(std::ostream& out, const Dimensions& dimensions)
{
	return out << "[ Width: " << dimensions.Width << "; Height: " << dimensions.Height << " ]";
}

namespace GraphicsEngine
{
	void Texture::Resize(int width, int height)
	{
		Size.Width = width;
		Size.Height = height;

		if (TextureID)
			glDeleteTextures(1, &TextureID); CheckGLErrors();

		glGenTextures(1, &TextureID); CheckGLErrors();
		glBindTexture(GL_TEXTURE_2D, TextureID); CheckGLErrors();

		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, DataType, nullptr); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, SampleType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, SampleType); CheckGLErrors();

		glBindTexture(GL_TEXTURE_2D, 0); CheckGLErrors();

	}

	void Texture::Resize(Dimensions size)
	{
		Resize(size.Width, size.Height);
	}

	void Texture::Load(int width, int height, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format, bool invertedY)
	{
		SampleType = sampleType;
		WrapType = wrapType;
		DataType = dataType;
		InternalFormat = internalFormat;
		Format = format;

		Size = Dimensions(width, height);
		InvertedY = invertedY;

		glGenTextures(1, &TextureID); CheckGLErrors();
		glBindTexture(GL_TEXTURE_2D, TextureID); CheckGLErrors();

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, NULL); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampleType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleType); CheckGLErrors();

		glBindTexture(GL_TEXTURE_2D, 0); CheckGLErrors();
	}

	namespace
	{
		unsigned char get(const char* str, int i)
		{
			return (unsigned char)(str[i]);
		}
	}

	void Texture::Load(const std::string& fileName, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format, bool isDXT5)
	{
		SampleType = sampleType;
		WrapType = wrapType;
		DataType = dataType;
		InternalFormat = internalFormat;
		Format = format;

		SoilTexture = true;
		InvertedY = true;
		IsDXT5nm = isDXT5;

		GLenum dtxFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		int blockSize = 16;
		unsigned int linearSize;
		unsigned int mipMapCount;

		if (isDXT5)
		{
			std::ifstream file(fileName, std::ios_base::in | std::ios_base::binary);

			if (!file.is_open() || !file.good())
				throw std::string("cannot open file: '" + fileName + "'");

			char headerType[4];

			file.read(headerType, 4);

			if (std::string(headerType, 4) != "DDS ")
				throw std::string("not a valid DDS: '" + fileName + "'");

			//char headerBits[4];
			//
			//file.Read(headerBits, 4);
			//
			//int headerSize = headerBits[0] | (headerBits[1] << 8) | (headerBits[2] << 16) | (headerBits[3] << 24);

			unsigned char header[124];

			file.read((char*)header, 124);

			Size.Width = (header[8]) | (header[9] << 8) | (header[10] << 16) | (header[11] << 24);
			Size.Height = (header[12]) | (header[13] << 8) | (header[14] << 16) | (header[15] << 24);
			linearSize = (header[16]) | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
			mipMapCount = (header[24]) | (header[25] << 8) | (header[26] << 16) | (header[27] << 24);

			if (mipMapCount == 0)
				mipMapCount = 1;

			unsigned int size = (mipMapCount > 1 ? linearSize + linearSize : linearSize);
			PixelData = new unsigned char[size];

			int count = -1;
			int remaining = size;
			char* offset = (char*)PixelData;

			while (count != 0 && remaining > 0)
			{
				file.read((char*)PixelData, remaining);
				count = int(file.gcount());
				remaining -= count;
				offset += count;
			}

			bool eof = file.eof();

			std::string formatName((char*)header + 80, 4);

			if (formatName == std::string("DXT1", 4))
			{
				blockSize = 8;
				dtxFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			}
			else if (formatName == std::string("DXT3", 4))
				dtxFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			else if (formatName == std::string("DXT5", 4))
				dtxFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			else
			{
				delete[] PixelData;

				throw "unsupported DDS: '" + fileName + "'";
			}
		}
		else
			PixelData = SOIL_load_image(fileName.c_str(), (int*)&Size.Width, (int*)&Size.Height, 0, SOIL_LOAD_RGBA);

		if (PixelData == nullptr)
			throw fileName + ": failed to load image\n";

		glGenTextures(1, &TextureID); CheckGLErrors();
		glBindTexture(GL_TEXTURE_2D, TextureID); CheckGLErrors();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType); CheckGLErrors();


		if (isDXT5)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampleType); CheckGLErrors();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleType); CheckGLErrors();


		if (isDXT5)
		{
			int widthI = Size.Width;
			int heightI = Size.Height;
			unsigned char* offset = PixelData;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount == 0 ? 0 : mipMapCount - 1); CheckGLErrors();

			for (int level = 0; (level < int(mipMapCount) && (widthI || heightI)) || (mipMapCount == 0 && level == 0); ++level)//&& level < int(mipMapCount) && (widthI > 0 || heightI > 0); ++level)
			{
				if (widthI < 4 || heightI < 4) {
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level == 0 ? 0 : level - 1); CheckGLErrors();

					break;
				}

				unsigned int size = ((widthI + 3) / 4) * ((heightI + 3) / 4) / 4 * blockSize;

				glCompressedTexImage2D(GL_TEXTURE_2D, level, dtxFormat, widthI, heightI, 0, widthI* heightI * blockSize / 16, offset); CheckGLErrors();

				offset += widthI * heightI * blockSize / 16;
				widthI /= 2;
				heightI /= 2;
			}
		}
		else
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, Size.Width, Size.Height, 0, format, dataType, PixelData);

		CheckGLErrors();
		glBindTexture(GL_TEXTURE_2D, 0);

		if (PixelData != nullptr)
		{
			if (SoilTexture && !IsDXT5nm)
				SOIL_free_image_data(PixelData);
			else if (!IsDXT5nm)
				delete PixelData;
			else
				delete[] PixelData;

			PixelData = nullptr;
		}
	}

	void Texture::Load(int width, int height, unsigned char* pixelData, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format, bool invertedY)
	{
		SampleType = sampleType;
		WrapType = wrapType;
		DataType = dataType;
		InternalFormat = internalFormat;
		Format = format;

		PixelData = pixelData;
		Size = Dimensions(width, height);
		InvertedY = invertedY;

		glGenTextures(1, &TextureID); CheckGLErrors();
		glBindTexture(GL_TEXTURE_2D, TextureID); CheckGLErrors();

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, pixelData); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampleType); CheckGLErrors();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampleType); CheckGLErrors();

		glBindTexture(GL_TEXTURE_2D, 0); CheckGLErrors();
	}

	Texture::~Texture()
	{
		if (PixelData != nullptr)
		{
			if (SoilTexture && !IsDXT5nm)
				SOIL_free_image_data(PixelData);
			else if (!IsDXT5nm)
				delete PixelData;
			else
				delete[] PixelData;
		}
	}

	void Texture::LoadPiece(const std::string& fileName, int x, int y, int width, int height, GLenum dataType, GLenum format)
	{
		int resWidth;
		int resHeight;

		unsigned char* data = SOIL_load_image(fileName.c_str(), &resWidth, &resHeight, 0, SOIL_LOAD_RGBA);

		if (data == nullptr)
			return;

		glBindTexture(GL_TEXTURE_2D, TextureID); CheckGLErrors();

		glPixelStorei(GL_UNPACK_SKIP_PIXELS, x); CheckGLErrors();
		glPixelStorei(GL_UNPACK_SKIP_ROWS, y); CheckGLErrors();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 16); CheckGLErrors();

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, dataType, data); CheckGLErrors();

		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0); CheckGLErrors();
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0); CheckGLErrors();
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); CheckGLErrors();

		glBindTexture(GL_TEXTURE_2D, 0); CheckGLErrors();

		SOIL_free_image_data(PixelData);
	}

	GLuint Texture::GetTextureID() const
	{
		return TextureID;
	}

	int Texture::GetWidth() const
	{
		return Size.Width;
	}

	int Texture::GetHeight() const
	{
		return Size.Height;
	}

	Vector3 Texture::GetResolution() const
	{
		return Vector3(float(Size.Width), float(Size.Height));
	}

	unsigned char* Texture::GetPixelData()
	{
		return PixelData;
	}

	RGBA Texture::GetPixel(int x, int y) const
	{
		if (PixelData == nullptr)
			return 0x00000000;

		if (x < 0)
			x += int(Size.Width);
		else if (x >= int(Size.Width))
			x -= int(Size.Width);

		if (y < 0)
			y += int(Size.Height);
		else if (y >= int(Size.Height))
			y -= int(Size.Height);

		unsigned int* pixel = reinterpret_cast<unsigned int*>(PixelData) + x + int(Size.Width) * y;

		return *pixel;
	}

	bool Texture::HasInvertedY() const
	{
		return InvertedY;
	}
}
