#include "CoreMeshes.h"

#include "MeshData.h"
#include "ObjParser.h"

namespace CoreMeshes
{
	const MeshData& Cone()
	{
		static MeshData mesh;
		static bool initialized = false;

		if (initialized)
			return mesh;

		initialized = true;

		ObjParser data;

		data.Vertices = {
			Vector3( 0.000000f,  0.000000f,  0.000000f, 1),
			Vector3(-0.198992f,  1.000000f,  1.000000f, 1),	
			Vector3( 0.198992f,  1.000000f,  1.000000f, 1),	
			Vector3( 0.566682f,  1.000000f,  0.848099f, 1),	
			Vector3( 0.848099f,  1.000000f,  0.566682f, 1),	
			Vector3( 1.000000f,  1.000000f,  0.198992f, 1),	
			Vector3( 1.000000f,  1.000000f, -0.198992f, 1),	
			Vector3( 0.848099f,  1.000000f, -0.566682f, 1),	
			Vector3( 0.566682f,  1.000000f, -0.848099f, 1),	
			Vector3( 0.198992f,  1.000000f, -1.000000f, 1),	
			Vector3(-0.198992f,  1.000000f, -1.000000f, 1),	
			Vector3(-0.566681f,  1.000000f, -0.848099f, 1),	
			Vector3(-0.848099f,  1.000000f, -0.566682f, 1),	
			Vector3(-1.000000f,  1.000000f, -0.198992f, 1),	
			Vector3(-1.000000f,  1.000000f,  0.198992f, 1),	
			Vector3(-0.848099f,  1.000000f,  0.566682f, 1),	
			Vector3(-0.566681f,  1.000000f,  0.848099f, 1)
		};

		data.Normals = {
			Vector3( 0.000000f, -0.707100f,  0.707100f),	
			Vector3( 0.270600f, -0.707100f,  0.653200f),	
			Vector3( 0.500000f, -0.707100f,  0.500000f),	
			Vector3( 0.653200f, -0.707100f,  0.270600f),	
			Vector3( 0.707100f, -0.707100f, -0.000000f),	
			Vector3( 0.653200f, -0.707100f, -0.270600f),	
			Vector3( 0.500000f, -0.707100f, -0.500000f),	
			Vector3( 0.270600f, -0.707100f, -0.653200f),	
			Vector3( 0.000000f, -0.707100f, -0.707100f),	
			Vector3(-0.270600f, -0.707100f, -0.653200f),	
			Vector3(-0.500000f, -0.707100f, -0.500000f),	
			Vector3(-0.653200f, -0.707100f, -0.270600f),	
			Vector3(-0.707100f, -0.707100f, -0.000000f),	
			Vector3(-0.653200f, -0.707100f,  0.270600f),	
			Vector3( 0.000000f, -1.000000f, -0.000000f),	
			Vector3(-0.500000f, -0.707100f,  0.500000f),	
			Vector3(-0.270600f, -0.707100f,  0.653200f),	
			Vector3( 0.000000f,  1.000000f,  0.000000f),	
			Vector3( 0.270700f, -0.707200f,  0.653100f),	
			Vector3( 0.499800f, -0.707200f,  0.500100f),	
			Vector3( 0.653300f, -0.707200f,  0.270400f),	
			Vector3( 0.653300f, -0.707200f, -0.270400f),	
			Vector3( 0.499900f, -0.707200f, -0.499900f),	
			Vector3( 0.271100f, -0.707100f, -0.653100f),	
			Vector3(-0.271100f, -0.707100f, -0.653100f),	
			Vector3(-0.500100f, -0.707100f, -0.499800f),	
			Vector3(-0.653300f, -0.707200f, -0.270400f),	
			Vector3(-0.707100f, -0.707200f, -0.000000f),	
			Vector3(-0.653300f, -0.707200f,  0.270400f),	
			Vector3(-0.270400f, -0.707200f,  0.653300f)
		};

		data.Colors = { { 1, 1, 1, 1 } };

		data.UVs = {
			Vector3(0, 0)
		};

		data.Faces = {
			Face{ { Vertex(  0, 0,   0, 0),Vertex(  2, 0,   0, 0),Vertex(  1, 0,   0, 0) } },	
			Face{ { Vertex(  0, 0,   1, 0),Vertex(  3, 0,   1, 0),Vertex(  2, 0,   1, 0) } },	
			Face{ { Vertex(  0, 0,   2, 0),Vertex(  4, 0,   2, 0),Vertex(  3, 0,   2, 0) } },	
			Face{ { Vertex(  0, 0,   3, 0),Vertex(  5, 0,   3, 0),Vertex(  4, 0,   3, 0) } },	
			Face{ { Vertex(  0, 0,   4, 0),Vertex(  6, 0,   4, 0),Vertex(  5, 0,   4, 0) } },	
			Face{ { Vertex(  0, 0,   5, 0),Vertex(  7, 0,   5, 0),Vertex(  6, 0,   5, 0) } },	
			Face{ { Vertex(  0, 0,   6, 0),Vertex(  8, 0,   6, 0),Vertex(  7, 0,   6, 0) } },	
			Face{ { Vertex(  0, 0,   7, 0),Vertex(  9, 0,   7, 0),Vertex(  8, 0,   7, 0) } },	
			Face{ { Vertex(  0, 0,   8, 0),Vertex( 10, 0,   8, 0),Vertex(  9, 0,   8, 0) } },	
			Face{ { Vertex( 10, 0,   9, 0),Vertex(  0, 0,   9, 0),Vertex( 11, 0,   9, 0) } },	
			Face{ { Vertex( 11, 0,  10, 0),Vertex(  0, 0,  10, 0),Vertex( 12, 0,  10, 0) } },	
			Face{ { Vertex(  0, 0,  11, 0),Vertex( 13, 0,  11, 0),Vertex( 12, 0,  11, 0) } },	
			Face{ { Vertex(  0, 0,  12, 0),Vertex( 14, 0,  12, 0),Vertex( 13, 0,  12, 0) } },	
			Face{ { Vertex( 14, 0,  13, 0),Vertex(  0, 0,  13, 0),Vertex( 15, 0,  13, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  15, 0),Vertex( 16, 0,  15, 0),Vertex( 15, 0,  15, 0) } },	
			Face{ { Vertex(  0, 0,  16, 0),Vertex(  1, 0,  16, 0),Vertex( 16, 0,  16, 0) } },	
			Face{ { Vertex(  8, 0,  17, 0),Vertex( 12, 0,  17, 0),Vertex( 16, 0,  17, 0) } },	
			Face{ { Vertex(  0, 0,   0, 0),Vertex(  0, 0,   0, 0),Vertex(  2, 0,   0, 0) } },	
			Face{ { Vertex(  0, 0,  18, 0),Vertex(  0, 0,  18, 0),Vertex(  3, 0,  18, 0) } },	
			Face{ { Vertex(  0, 0,  19, 0),Vertex(  0, 0,  19, 0),Vertex(  4, 0,  19, 0) } },	
			Face{ { Vertex(  0, 0,  20, 0),Vertex(  0, 0,  20, 0),Vertex(  5, 0,  20, 0) } },	
			Face{ { Vertex(  0, 0,   4, 0),Vertex(  0, 0,   4, 0),Vertex(  6, 0,   4, 0) } },	
			Face{ { Vertex(  0, 0,  21, 0),Vertex(  0, 0,  21, 0),Vertex(  7, 0,  21, 0) } },	
			Face{ { Vertex(  0, 0,  22, 0),Vertex(  0, 0,  22, 0),Vertex(  8, 0,  22, 0) } },	
			Face{ { Vertex(  0, 0,  23, 0),Vertex(  0, 0,  23, 0),Vertex(  9, 0,  23, 0) } },	
			Face{ { Vertex(  0, 0,   8, 0),Vertex(  0, 0,   8, 0),Vertex( 10, 0,   8, 0) } },	
			Face{ { Vertex( 10, 0,  24, 0),Vertex(  0, 0,  24, 0),Vertex(  0, 0,  24, 0) } },	
			Face{ { Vertex( 11, 0,  25, 0),Vertex(  0, 0,  25, 0),Vertex(  0, 0,  25, 0) } },	
			Face{ { Vertex(  0, 0,  26, 0),Vertex(  0, 0,  26, 0),Vertex( 13, 0,  26, 0) } },	
			Face{ { Vertex(  0, 0,  27, 0),Vertex(  0, 0,  27, 0),Vertex( 14, 0,  27, 0) } },	
			Face{ { Vertex( 14, 0,  28, 0),Vertex(  0, 0,  28, 0),Vertex(  0, 0,  28, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0),Vertex(  0, 0,  14, 0) } },	
			Face{ { Vertex(  0, 0,  15, 0),Vertex(  0, 0,  15, 0),Vertex( 16, 0,  15, 0) } },	
			Face{ { Vertex(  0, 0,  29, 0),Vertex(  0, 0,  29, 0),Vertex(  1, 0,  29, 0) } },	
			Face{ { Vertex( 16, 0,  17, 0),Vertex(  1, 0,  17, 0),Vertex(  4, 0,  17, 0) } },	
			Face{ { Vertex(  2, 0,  17, 0),Vertex(  3, 0,  17, 0),Vertex(  4, 0,  17, 0) } },	
			Face{ { Vertex(  4, 0,  17, 0),Vertex(  5, 0,  17, 0),Vertex(  6, 0,  17, 0) } },	
			Face{ { Vertex(  6, 0,  17, 0),Vertex(  7, 0,  17, 0),Vertex(  8, 0,  17, 0) } },	
			Face{ { Vertex(  8, 0,  17, 0),Vertex(  9, 0,  17, 0),Vertex( 10, 0,  17, 0) } },	
			Face{ { Vertex( 10, 0,  17, 0),Vertex( 11, 0,  17, 0),Vertex( 12, 0,  17, 0) } },	
			Face{ { Vertex( 12, 0,  17, 0),Vertex( 13, 0,  17, 0),Vertex( 14, 0,  17, 0) } },	
			Face{ { Vertex( 14, 0,  17, 0),Vertex( 15, 0,  17, 0),Vertex( 12, 0,  17, 0) } },	
			Face{ { Vertex(  1, 0,  17, 0),Vertex(  2, 0,  17, 0),Vertex(  4, 0,  17, 0) } },	
			Face{ { Vertex(  4, 0,  17, 0),Vertex(  6, 0,  17, 0),Vertex( 16, 0,  17, 0) } },	
			Face{ { Vertex(  8, 0,  17, 0),Vertex( 10, 0,  17, 0),Vertex( 12, 0,  17, 0) } },	
			Face{ { Vertex( 12, 0,  17, 0),Vertex( 15, 0,  17, 0),Vertex( 16, 0,  17, 0) } },	
			Face{ { Vertex( 16, 0,  17, 0),Vertex(  6, 0,  17, 0),Vertex(  8, 0,  17, 0) } }
		};

		mesh = MeshData(&data, Enum::VertexMode::Seperate);

		return mesh;
	}
}