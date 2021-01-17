#include "VertexModeEnum.h"

#include "ObjectReflection.h"
#include "ObjectLuaType.h"

namespace Engine
{
	Enum_Definition(VertexMode,
		Document_Enum("");
		
		Enum_Item(Seperate);
		Enum_Item(Shared);
		Enum_Item(Smoothened);
		Enum_Item(Wireframe);
	);
}