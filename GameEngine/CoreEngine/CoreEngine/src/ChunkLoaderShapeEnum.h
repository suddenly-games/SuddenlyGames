#pragma once

#include "EnumDeclaration.h"

namespace EnumTypeStorage
{
	struct ChunkLoaderShapeEnum
	{
		enum ChunkLoaderShape
		{
			Box,
			Sphere,
			CylinderX,
			CylinderY,
			CylinderZ
		};
	};
}

namespace Enum
{
	typedef EnumTypeStorage::ChunkLoaderShapeEnum::ChunkLoaderShape ChunkLoaderShape;
}

namespace Engine
{
	Declare_Enum(ChunkLoaderShape);
}
