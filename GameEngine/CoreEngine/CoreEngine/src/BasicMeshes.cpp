#include "CoreMeshes.h"

#include "MeshData.h"
#include "ObjParser.h"

namespace CoreMeshes
{
	const MeshData& Square()
	{
		static MeshData mesh;
		static bool initialized = false;

		if (initialized)
			return mesh;

		initialized = true;

		ObjParser data;

		data.Vertices = {
			Vector3(1, -1, 0, 1),
			Vector3(-1, -1, 0, 1),
			Vector3(1,  1, 0, 1),
			Vector3(-1,  1, 0, 1)
		};

		data.Normals = {
			Vector3(0, 0, 1)
		};

		data.Colors = { { 1, 1, 1, 1 } };

		data.UVs = {
			Vector3(1, 0),
			Vector3(1, 1),
			Vector3(0, 0),
			Vector3(0, 1)
		};

		data.Faces = {
			Face{ { Vertex(0, 0, 0, 0),Vertex(2, 1, 0, 0),Vertex(1, 2, 0, 0) } },
			Face{ { Vertex(2, 1, 0, 0),Vertex(3, 3, 0, 0),Vertex(1, 2, 0, 0) } }
		};

		mesh = MeshData(&data, Enum::VertexMode::Seperate);

		return mesh;
	}

	const MeshData& Cube()
	{
		static MeshData mesh;
		static bool initialized = false;

		if (initialized)
			return mesh;

		initialized = true;

		ObjParser data;

		data.Vertices = {
			Vector3(1, -1, -1, 1),
			Vector3(1, -1,  1, 1),
			Vector3(-1, -1,  1, 1),
			Vector3(-1, -1, -1, 1),
			Vector3(1,  1, -1, 1),
			Vector3(1,  1,  1, 1),
			Vector3(-1,  1,  1, 1),
			Vector3(-1,  1, -1, 1)
		};

		data.Normals = {
			Vector3(0, -1,  0),
			Vector3(0,  1,  0),
			Vector3(1,  0,  0),
			Vector3(0,  0,  1),
			Vector3(-1,  0,  0),
			Vector3(0,  0, -1)
		};

		data.Colors = { { 1, 1, 1, 1 } };

		data.UVs = {
			Vector3(0, 0),
			Vector3(1, 0),
			Vector3(0, 1),
			Vector3(1, 1)
		};

		data.Faces = {
			Face{ { Vertex(0, 0, 5, 0),Vertex(7, 3, 5, 0),Vertex(4, 2, 5, 0) }, "" },
			Face{ { Vertex(0, 0, 5, 0),Vertex(3, 1, 5, 0),Vertex(7, 3, 5, 0) }, "" },
			Face{ { Vertex(5, 3, 3, 0),Vertex(2, 0, 3, 0),Vertex(1, 1, 3, 0) }, "" },
			Face{ { Vertex(5, 3, 3, 0),Vertex(6, 2, 3, 0),Vertex(2, 0, 3, 0) }, "" },
			Face{ { Vertex(4, 3, 2, 0),Vertex(1, 0, 2, 0),Vertex(0, 1, 2, 0) }, "" },
			Face{ { Vertex(4, 3, 2, 0),Vertex(5, 2, 2, 0),Vertex(1, 0, 2, 0) }, "" },
			Face{ { Vertex(2, 1, 4, 0),Vertex(7, 2, 4, 0),Vertex(3, 0, 4, 0) }, "" },
			Face{ { Vertex(2, 1, 4, 0),Vertex(6, 3, 4, 0),Vertex(7, 2, 4, 0) }, "" },
			Face{ { Vertex(7, 2, 1, 0),Vertex(5, 1, 1, 0),Vertex(4, 3, 1, 0) }, "" },
			Face{ { Vertex(7, 2, 1, 0),Vertex(6, 0, 1, 0),Vertex(5, 1, 1, 0) }, "" },
			Face{ { Vertex(1, 3, 0, 0),Vertex(3, 0, 0, 0),Vertex(0, 1, 0, 0) }, "" },
			Face{ { Vertex(1, 3, 0, 0),Vertex(2, 2, 0, 0),Vertex(3, 0, 0, 0) }, "" }
		};

		mesh = MeshData(&data, Enum::VertexMode::Seperate);

		return mesh;
	}

	const MeshData& WireCube()
	{
		static MeshData mesh;
		static bool initialized = false;

		if (initialized)
			return mesh;

		initialized = true;

		mesh.DrawMode = GL_LINES;

		Vector3 vertices[8] = {
			Vector3(1, -1, -1, 1),
			Vector3(1, -1,  1, 1),
			Vector3(-1, -1,  1, 1),
			Vector3(-1, -1, -1, 1),
			Vector3(1,  1, -1, 1),
			Vector3(1,  1,  1, 1),
			Vector3(-1,  1,  1, 1),
			Vector3(-1,  1, -1, 1)
		};

		for (int i = 0; i < 8; ++i)
		{
			VertexData vertex;

			vertex.Position = vertices[i];

			mesh.VertexBuffer.push_back(vertex);
		}

		mesh.IndexBuffer.push_back(0);
		mesh.IndexBuffer.push_back(1);
		mesh.IndexBuffer.push_back(2);
		mesh.IndexBuffer.push_back(3);
		mesh.IndexBuffer.push_back(4);
		mesh.IndexBuffer.push_back(5);
		mesh.IndexBuffer.push_back(6);
		mesh.IndexBuffer.push_back(7);

		mesh.IndexBuffer.push_back(0);
		mesh.IndexBuffer.push_back(3);
		mesh.IndexBuffer.push_back(4);
		mesh.IndexBuffer.push_back(7);
		mesh.IndexBuffer.push_back(1);
		mesh.IndexBuffer.push_back(2);
		mesh.IndexBuffer.push_back(5);
		mesh.IndexBuffer.push_back(6);

		mesh.IndexBuffer.push_back(0);
		mesh.IndexBuffer.push_back(4);
		mesh.IndexBuffer.push_back(1);
		mesh.IndexBuffer.push_back(5);
		mesh.IndexBuffer.push_back(2);
		mesh.IndexBuffer.push_back(6);
		mesh.IndexBuffer.push_back(3);
		mesh.IndexBuffer.push_back(7);

		mesh.UpdateBounds();

		return mesh;
	}
}