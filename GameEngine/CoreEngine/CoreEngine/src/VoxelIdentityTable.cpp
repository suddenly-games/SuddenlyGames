#include "VoxelTriangulation.h"

namespace Engine
{
	void VoxelTriangulation::InitializeIdentities()
	{
		TriangulationIdentities.reserve(17);

		// 0
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));
		}

		// 1
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0, -1, 1)
			));
		}

		// 2
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  0,  1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  0,  1, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0, -1, 1)
			));
		}

		// 3
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  1,  0, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 0,  1, -1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 0,  1, -1, 1)
			));
		}

		// 4
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0,  1, 1),
				Vector3( 0,  1,  1, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3( 1, -1,  0, 1),
				Vector3( 1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 0, -1, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  1,  0, 1),
				Vector3(-1,  0, -1, 1)
			));
		}

		// 5
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1,  1,  0, 1),
				Vector3(-1,  0, -1, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1, -1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));
		}

		// 6
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3(-1, -1,  0, 1),
				Vector3(-1,  0,  1, 1)
			));
		}

		// 7
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3(-1, -1,  0, 1),
				Vector3(-1,  0,  1, 1)
			));
		}

		// 8
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3(-1,  0, -1, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3( 1,  0,  1, 1),
				Vector3( 1, -1,  0, 1)
			));
		}

		// 9
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0,  1,  1, 1),
				Vector3(-1,  0,  1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3(-1,  0, -1, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3( 1,  0,  1, 1),
				Vector3( 1, -1,  0, 1)
			));
		}

		// 10
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 1,  0,  1, 1),
				Vector3( 0,  1,  1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1,  0,  1, 1),
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3(-1,  1,  0, 1),
				Vector3(-1, -1,  0, 1)
			));
		}

		// 11
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 1,  1,  0, 1),
				Vector3( 0,  1,  1, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 0,  1,  1, 1),
				Vector3(-1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  0,  1, 1),
				Vector3( 0, -1, -1, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  0, -1, 1),
				Vector3( 0, -1, -1, 1),
				Vector3(-1,  0,  1, 1)
			));
		}

		// 12
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  1,  0, 1),
				Vector3(-1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 0,  1, -1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 1, -1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 0,  1, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 0,  1,  1, 1),
				Vector3( 1,  1,  0, 1),
				Vector3( 1,  0,  1, 1)
			));
		}

		// 13
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3(-1, -1,  0, 1),
				Vector3( 1,  0,  1, 1),
				Vector3( 1, -1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1, -1,  0, 1),
				Vector3(-1,  0,  1, 1),
				Vector3( 1,  0,  1, 1)
			));
		}

		// 14
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1,  1, 1),
				Vector3(-1,  1,  0, 1),
				Vector3( 1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0,  1, 1),
				Vector3(-1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  0, -1, 1),
				Vector3( 1, -1,  0, 1),
				Vector3( 1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3( 1, -1,  0, 1),
				Vector3(-1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3(-1, -1,  0, 1),
				Vector3(-1,  0,  1, 1)
			));
		}

		// 15
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3( 0,  1, -1, 1),
				Vector3( 1,  1,  0, 1),
				Vector3( 1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1, -1, 1),
				Vector3(-1, -1,  0, 1),
				Vector3(-1,  0, -1, 1)
			));
		}

		// 16
		{
			TriangulationIdentities.push_back(Identity());

			FaceVector& faces = TriangulationIdentities.back().Faces;

			faces.push_back(Face(
				Vector3(-1,  1,  0, 1),
				Vector3(-1,  0,  1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3( 1,  1,  0, 1),
				Vector3(-1,  0,  1, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3( 1,  1,  0, 1)
			));

			faces.push_back(Face(
				Vector3( 0, -1,  1, 1),
				Vector3(-1, -1,  0, 1),
				Vector3( 1,  0, -1, 1)
			));

			faces.push_back(Face(
				Vector3(-1,  0, -1, 1),
				Vector3( 1,  0, -1, 1),
				Vector3(-1, -1,  0, 1)
			));
		}
	}
}