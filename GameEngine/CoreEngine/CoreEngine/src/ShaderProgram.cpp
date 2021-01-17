#include "ShaderProgram.h"

#include <functional>

#include "Mesh.h"
#include "Uniform.h"

#include "Graphics.h"

ShaderProgram* ShaderProgram::CurrentProgram = nullptr;

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ProgramID); CheckGLErrors();
}

GLint ShaderProgram::RegisterUniform(UniformBase* uniform)
{
	GLint handle = glGetUniformLocation(ProgramID, uniform->GetName().c_str()); CheckGLErrors();

	Uniforms[handle] = uniform;
	UniformIDs[uniform->GetName()] = handle;

	return handle;
}

void ShaderProgram::Use()
{
	if (CurrentProgram == this)
		return;

	CurrentProgram = this;

	glUseProgram(ProgramID); CheckGLErrors();
}

GLuint ShaderProgram::GetProgramID() const
{
	return ProgramID;
}

const std::string& ShaderProgram::GetName() const
{
	return Name;
}

void ShaderProgram::InitAttributes() const
{
	InitAttribute("vertexPosition", 0, 3);
	InitAttribute("vertexNormal", 3, 3);
	InitAttribute("vertexUV", 6, 2);
	InitAttribute("vertexTVector", 8, 3);
	InitAttribute("vertexBVector", 11, 3);
}

ShaderProgram* ShaderProgram::GetCurrentProgram()
{
	return CurrentProgram;
}

void ShaderProgram::InitAttribute(const std::string& attribute, int offset, int size) const
{
	GLuint location = glGetAttribLocation(ProgramID, attribute.c_str()); CheckGLErrors();

	if (location != -1)
	{
		glEnableVertexAttribArray(location); CheckGLErrors();
		glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(offset * sizeof(float))); CheckGLErrors();
	}
}

void ShaderProgram::Initialize()
{
	ProgramID = glCreateProgram(); CheckGLErrors();

	glBindAttribLocation(ProgramID, 0, "vertexPosition"); CheckGLErrors();
	glBindAttribLocation(ProgramID, 1, "vertexNormal"); CheckGLErrors();
	glBindAttribLocation(ProgramID, 2, "vertexUV"); CheckGLErrors();
	glBindAttribLocation(ProgramID, 3, "vertexColor"); CheckGLErrors();
	glBindAttribLocation(ProgramID, 4, "vertexTVector"); CheckGLErrors();
	glBindAttribLocation(ProgramID, 5, "vertexBVector"); CheckGLErrors();
}

void ShaderProgram::AttachProgramShader(Shader* shader)
{
	glAttachShader(ProgramID, shader->GetShaderID()); CheckGLErrors();
}

void ShaderProgram::Link()
{
	glLinkProgram(ProgramID); CheckGLErrors();

	CheckErrors(ProgramID, GL_LINK_STATUS);

	Use();

	//Meshes.InitLoadedMeshes();
}

void ShaderProgram::CheckErrors(GLuint data, int action) const
{
	GLint result;

	glGetProgramiv(data, action, &result);

	if (result == GL_FALSE)
	{
		int length;

		glGetProgramiv(data, GL_INFO_LOG_LENGTH, &length);

		char* outputInfo = new char[length];

		glGetProgramInfoLog(data, length, 0, outputInfo);

		std::string output;
		output.append(outputInfo, length);

		delete[] outputInfo;

		throw output;
	}
}

ProgramInitializer::ProgramInitializer(ShaderProgram* program, const std::string& name)
{
	std::function< std::function<int(bool)>(std::function<int(bool)>)> callback = [](std::function<int(bool)> stupidArg) -> std::function<int(bool)> { return [](bool arg) -> int { return arg ? 3 : 5; }; };

	program->Name = name;

	program->Initialize();
}

ProgramAttacher::ProgramAttacher(ShaderProgram* program, const std::string& shaderName)
{
	program->AttachProgramShader(Shader::GetShader(shaderName));
}

ProgramLinker::ProgramLinker(ShaderProgram* program)
{
	program->Link();
}
