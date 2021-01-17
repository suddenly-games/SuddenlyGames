#include "ColliderAsset.h"

namespace Engine
{
	namespace Physics
	{
		void ColliderAsset::ConfigureTriangleMesh()
		{
			Data.Push(Vector3( 0,  1));
			Data.Push(Vector3( 1, -1));
			Data.Push(Vector3(-1, -1));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureSquareMesh()
		{
			Data.Push(Vector3( 1,  1));
			Data.Push(Vector3( 1, -1));
			Data.Push(Vector3(-1, -1));
			Data.Push(Vector3(-1,  1));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureOctagonMesh()
		{
			float scale = 1.0f / std::cos(3.1415f / 16);

			for (int i = 0; i < 8; ++i)
				Data.Push(Vector3(scale *  std::sin(2 * 3.1415f * float(i) / 8), scale * std::cos(2 * 3.1415f * float(i) / 8)));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureShipMesh()
		{
			Data.Push(Vector3(-0.18f,  0.8f));
			Data.Push(Vector3( 0.18f,  0.8f));
			Data.Push(Vector3( 1.00f, -0.8f));
			Data.Push(Vector3(-1.00f, -0.8f));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureChargerShieldMesh()
		{
			Data.Push(Vector3(-0.45f,  1.00f));
			Data.Push(Vector3( 0.45f,  1.00f));
			Data.Push(Vector3( 1.0f, -0.25f));
			Data.Push(Vector3( 0.0f, -1.00f));
			Data.Push(Vector3(-1.0f, -0.25f));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureWormHeadMesh()
		{
			Data.Push(Vector3(-0.8f, -1.00f));
			Data.Push(Vector3(-1.0f,  0.05f));
			Data.Push(Vector3( 0.0f,  1.00f));
			Data.Push(Vector3( 1.0f,  0.05f));
			Data.Push(Vector3( 0.8f, -1.00f));

			Data.Finalize();
		}

		void ColliderAsset::ConfigureWormTailMesh()
		{
			Data.Push(Vector3(-1.0f,  1.00f));
			Data.Push(Vector3( 1.0f,  1.00f));
			Data.Push(Vector3( 0.9f,  0.34f));
			Data.Push(Vector3( 0.0f, -1.00f));
			Data.Push(Vector3(-0.9f,  0.34f));

			Data.Finalize();
		}
	}
}