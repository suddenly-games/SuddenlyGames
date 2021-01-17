#pragma once

#include "Object.h"
#include "ColliderData.h"

namespace Engine
{
	namespace Physics
	{
		class ColliderAsset : public Object
		{
		public:
			ColliderData Data;

			~ColliderAsset() {}

			void Initialize() {}
			void Update(float) {}

			void ConfigureTriangleMesh();
			void ConfigureSquareMesh();
			void ConfigureOctagonMesh();
			void ConfigureShipMesh();
			void ConfigureChargerShieldMesh();
			void ConfigureWormHeadMesh();
			void ConfigureWormTailMesh();

			Instantiable;

			Inherits_Class(Object);

			Reflected(ColliderAsset);
		};
	}

	//Class_Inherits(Physics::ColliderAsset, Object);
}