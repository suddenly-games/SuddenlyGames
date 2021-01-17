#include "Mesh.h"

#include "ShaderProgram.h"
#include "Graphics.h"
#include "MeshData.h"

const Mesh* Mesh::BoundMesh = nullptr;

Mesh::Mesh(const MeshData* data, const ShaderProgram* program, int id) : DataID(id)
{
	DrawMode = data->DrawMode;

	Load(data, program);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VertexBuffer); CheckGLErrors();
	glDeleteBuffers(1, &IndexBuffer); CheckGLErrors();
	glDeleteVertexArrays(1, &MeshID); CheckGLErrors();
}

GLuint Mesh::GetMeshID() const
{
	return MeshID;
}

int Mesh::GetDataID() const
{
	return DataID;
}

GLuint Mesh::GetIndicesID() const
{
	return Indices;
}

const ShaderProgram* Mesh::GetProgram() const
{
	return Program;
}

void Mesh::Draw() const
{
	if (BoundMesh != this)
	{
		BoundMesh = this;

		glBindVertexArray(MeshID); CheckGLErrors();
	}

	//_program->UseProgram();
	glDrawElements(DrawMode, Indices, GL_UNSIGNED_INT, 0); CheckGLErrors();
	//glBindVertexArray(-1);
}

void Mesh::Load(const MeshData* data, const ShaderProgram* program)
{
	program = program;
	Indices = data->IndexBuffer.size();

	glGenVertexArrays(1, &MeshID); CheckGLErrors();
	glBindVertexArray(MeshID); CheckGLErrors();

	glGenBuffers(1, &VertexBuffer); CheckGLErrors();
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); CheckGLErrors();
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * data->VertexBuffer.size(), data->VertexBuffer.data(), GL_STATIC_DRAW); CheckGLErrors();

	glGenBuffers(1, &IndexBuffer); CheckGLErrors();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer); CheckGLErrors();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->IndexBuffer.size(), data->IndexBuffer.data(), GL_STATIC_DRAW); CheckGLErrors();

	program->InitAttributes();

	glBindBuffer(GL_ARRAY_BUFFER, 0); CheckGLErrors();
	glBindVertexArray(0); CheckGLErrors();
}

const std::string& Mesh::GetName() const
{
	return Name;
}

void Mesh::Verify()
{
	if (Program == nullptr)
		throw "mesh not loaded";
}
