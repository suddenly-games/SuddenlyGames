#include "Light.h"

namespace Engine
{
	using Enum::LightType;

	Enum_Definition(LightType,
		Document_Enum("");
		
		Enum_Item(Directional);
		Enum_Item(Point);
		Enum_Item(Spot);
	);
}