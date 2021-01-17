#pragma once

#include "ShaderProgram.h"

#define Uniform(type, name) type name = type(std::string(#name), std::string(#type), this)
#define Constructor(type) type(const std::string& name, const std::string& type, ShaderProgram* program) : UniformBase(name, type, program) {}

class UniformBase
{
public:
	UniformBase(const std::string& name, const std::string& type, ShaderProgram* program);

	GLint GetHandle() const;
	const std::string& GetName() const;
	ShaderProgram* GetProgram() { return Program; }
	void Validate();

private:
	ShaderProgram* Program = nullptr;
	GLint Handle = -1;
	std::string Name;
	std::string Type;
};

namespace GLSL
{
	class Bool : public UniformBase
	{
	public:
		Constructor(Bool);

		bool Set(bool value);

	private:
		bool CurrentValue = false;
		bool Initialized = false;
	};

	class Int : public UniformBase
	{
	public:
		Constructor(Int);

		bool Set(int value);
	};

	class Float : public UniformBase
	{
	public:
		Constructor(Float);

		bool Set(float value);
	};

	class Vec2 : public UniformBase
	{
	public:
		Constructor(Vec2);

		bool Set(const Vector3& vector);
		bool Set(float x, float y);
	};

	class Vec3 : public UniformBase
	{
	public:
		Constructor(Vec3);

		bool Set(const Vector3& vector);
		bool Set(const RGBA& color);
		bool Set(float x, float y, float z);
		bool Set(unsigned int color);

	private:
		Vector3 CurrentValues;
	};

	class Vec4 : public UniformBase
	{
	public:
		Constructor(Vec4);

		bool Set(const Vector3& vector);
		bool Set(const RGBA& color);
		bool Set(float x, float y, float z, float w);
		bool Set(unsigned int color);

	private:
		Vector3 CurrentValues;
	};

	class Matrix : public UniformBase
	{
	public:
		Constructor(Matrix);

		bool Set(const Matrix3& matrix);

	private:
		Matrix3 CurrentValue;
		bool Initialized = false;
	};

	class Sampler2D : public UniformBase
	{
	public:
		Constructor(Sampler2D);

		bool Set(GLuint texture, int sampler = 0);
		bool Set(const std::shared_ptr<GraphicsEngine::Texture>& texture, int sampler = 0);

	private:
		struct SamplerData
		{
			bool BoundTexture = false;
			GLuint CurrentTextureID = 0;
		};

		typedef std::vector<SamplerData> SamplerVector;

		static int BoundSampler;
		static SamplerVector Samplers;

		int CurrentSampler = -1;
	};

	class IntArray : public UniformBase
	{
	public:
		Constructor(IntArray);

		bool Set(const int* values, int size);
	};

	class FloatArray : public UniformBase
	{
	public:
		Constructor(FloatArray);

		bool Set(const float* values, int size);
	};

	class Vec2Array : public UniformBase
	{
	public:
		Constructor(Vec2Array);

		bool Set(const float* data, int size);
		bool Set(const Vector3* vector, int size);
	};

	class Vec3Array : public UniformBase
	{
	public:
		Constructor(Vec3Array);

		bool Set(const float* data, int size);
		bool Set(const Vector3* vector, int size);
	};
}
