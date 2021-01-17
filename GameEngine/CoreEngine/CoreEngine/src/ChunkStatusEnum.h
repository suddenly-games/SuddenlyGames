#pragma once

#include "EnumDeclaration.h"

namespace EnumTypeStorage
{
	struct ChunkStatusEnum
	{
		enum ChunkStatus
		{
			Unloaded,
			NotGenerated,
			Loading,
			Loaded
		};
	};
}

namespace Enum
{
	typedef EnumTypeStorage::ChunkStatusEnum::ChunkStatus ChunkStatus;
}

namespace Engine
{
	Declare_Enum(ChunkStatus);
}