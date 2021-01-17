#include "ChunkLoaderShapeEnum.h"

#include "ObjectReflection.h"
#include "ObjectLuaType.h"

namespace Engine
{
	Enum_Definition(ChunkLoaderShape,
		Document_Enum("");
		
		Enum_Item(Box);
		Enum_Item(Sphere);
		Enum_Item(CylinderX);
		Enum_Item(CylinderY);
		Enum_Item(CylinderZ);
	);
}