#include "Shader.h"

#include <fstream>

#include "ShaderProgram.h"
#include "Graphics.h"

Shader::ShaderMap Shader::Shaders = Shader::ShaderMap();

Shader::Shader() : ShaderID(-1) {}

Shader::Shader(const std::string& filePath, const std::string& name, ShaderType type) : Name(name)
{
	std::string source;

	ReadSource(filePath, source);

	const char* sourceCode = source.c_str();

	ShaderID = glCreateShader(type); CheckGLErrors();

	glShaderSource(ShaderID, 1, &sourceCode, 0); CheckGLErrors();

	glCompileShader(ShaderID); CheckGLErrors();

	try
	{
		CheckErrors(ShaderID, GL_COMPILE_STATUS);
	}
	catch (std::string& error)
	{
		throw filePath + ": " + error;
	}

	Shaders[name] = this;
}

void Shader::Compile(const std::string& filePath, int shaderType)
{
	if (ShaderID != -1)
		throw "attempt to compile shader that is in use";

	std::string source;

	ReadSource(filePath, source);

	const char* sourceCode = source.c_str();

	ShaderID = glCreateShader(shaderType); CheckGLErrors();
	

	glShaderSource(ShaderID, 1, &sourceCode, 0); CheckGLErrors();// glShaderSource(s)
	
	glCompileShader(ShaderID); CheckGLErrors();
	

	try
	{
		CheckErrors(ShaderID, GL_COMPILE_STATUS);
		
	}
	catch (std::string& error)
	{
		throw filePath + ": " + error;
	}
}

Shader::~Shader()
{
	Shaders.erase(Name);

	glDeleteShader(ShaderID); CheckGLErrors();
}

GLuint Shader::GetShaderID() const
{
	return ShaderID;
}

Shader* Shader::GetShader(const std::string& name)
{
	ShaderMap::iterator shader = Shaders.find(name);

	if (shader != Shaders.end())
		return shader->second;

	return nullptr;
}

template <int bufferSize>
void Shader::ReadSource(const std::string& filePath, std::string& source)
{
	std::fstream file(filePath, std::ios_base::in);

	if (!file.is_open() || !file.good())
	{
		std::string error = "cannot open file: '";
		error += filePath + "'\n";
		error += std::string("eof: ").append({ '0' + char(file.eof() )}) + "\n";
		error += std::string("bad: ").append({ '0' + char(file.bad() )}) + "\n";
		error += std::string("fail: ").append({ '0' + char(file.fail() )}) + "\n";

		throw error;

	}
	char buffer[bufferSize] = {};

	while (!file.eof())
	{
		file.read(buffer, bufferSize);

		int read = int(file.gcount());

		source.append(buffer, read);
	}
}

namespace Testing
{
	typedef std::vector<char> String;

	void copy(String& string, int length, const char* source, int size)
	{
		if (int(string.size()) < length)
			string.resize(string.size() * 2);

		for (int i = length; i < (length + size); ++i)
			string[i] = source[i - length];
	}

	namespace ParseState
	{
		enum ParseState
		{
			seek,
			newLine,
			include,
			whiteSpace1
		};
	}

	template <int bufferSize>
	void readSource(const std::string& filePath, std::string& source)
	{
		std::fstream file(filePath, std::ios_base::in);

		if (!file.is_open() || !file.good())
			throw std::string("cannot open file: '") + filePath + "'";

		String fileOut(bufferSize * 2);

		char buffer[bufferSize];

		while (!file.eof())
		{
			file.read(buffer, bufferSize);

			int read = int(file.gcount());


		}
	}
}

void Shader::CheckErrors(GLuint data, int action)
{
	GLint result;

	glGetShaderiv(data, action, &result);

	if (result == GL_FALSE)
	{
		int length;

		glGetShaderiv(data, GL_INFO_LOG_LENGTH, &length);

		char* outputInfo = new char[length];

		glGetShaderInfoLog(data, length, 0, outputInfo);

		std::string output;
		output.append(outputInfo, length);

		delete[] outputInfo;

		throw output;
	}
}
