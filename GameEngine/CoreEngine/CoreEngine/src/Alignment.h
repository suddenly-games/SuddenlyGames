#pragma once

#include "EnumDeclaration.h"

namespace GraphicsEngine
{
	struct AlignmentEnum
	{
		enum Alignment
		{
			Minimum,
			Center,
			Maximum
		};
	};
}

namespace Enum
{
	typedef GraphicsEngine::AlignmentEnum::Alignment Alignment;
}

namespace Engine
{
	Declare_Enum(Alignment);
}