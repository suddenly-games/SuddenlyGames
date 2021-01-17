#pragma once

extern "C" {
#include "glew.h"
}
#include "Matrix3.h"

class MeshData;

class ShaderProgram;

class Mesh
{
public:
	~Mesh();

	GLuint GetMeshID() const;
	int GetDataID() const;
	GLuint GetIndicesID() const;
	const ShaderProgram* GetProgram() const;
	void Draw() const;
	void Load(const MeshData* data, const ShaderProgram* program);
	const std::string& GetName() const;

	GLenum DrawMode = GL_TRIANGLES;

	friend class MeshLoader;

private:
	std::string Name;
	int DataID;
	GLuint MeshID, VertexBuffer, IndexBuffer;
	const ShaderProgram* Program;
	int Indices;

	Mesh(const MeshData* data, const ShaderProgram* program, int id);

	static const Mesh* BoundMesh;

	void Verify();
};
