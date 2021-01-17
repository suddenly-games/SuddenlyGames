#pragma once

#include "EnumDeclaration.h"

namespace EnumTypeStorage
{
	struct VertexModeEnum
	{
		enum VertexMode
		{
			Seperate,
			Shared,
			Smoothened,
			Wireframe
		};
	};
}

namespace Enum
{
	typedef EnumTypeStorage::VertexModeEnum::VertexMode VertexMode;
}

namespace Engine
{
	Declare_Enum(VertexMode);
}