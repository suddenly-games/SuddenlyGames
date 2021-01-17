#include "ChunkStatusEnum.h"

#include "ObjectReflection.h"
#include "ObjectLuaType.h"

namespace Engine
{
	Enum_Definition(ChunkStatus,
		Document_Enum("");
		
		Enum_Item(Unloaded);
		Enum_Item(NotGenerated);
		Enum_Item(Loading);
		Enum_Item(Loaded);
	);
}