#pragma once

#include <string>
#include <map>
#include <fstream>

extern "C" {
#include "glew.h"
}

class Shader
{
public:
	enum ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};

	Shader();
	Shader(const std::string& filePath, const std::string& name, ShaderType type);
	~Shader();

	void Compile(const std::string& filePath, int shaderType);
	GLuint GetShaderID() const;

	static Shader* GetShader(const std::string& name);

private:
	typedef std::map<std::string, Shader*> ShaderMap;

	GLuint ShaderID;
	std::string Name;

	static ShaderMap Shaders;

	template <int bufferSize = 4096>
	void ReadSource(const std::string& filePath, std::string& source);

	void CheckErrors(GLuint data, int action);
};

namespace Enum
{
	typedef Shader::ShaderType ShaderType;
}

typedef Shader::ShaderType ShaderType;
