#include "EnumTemplate.h"
#include "Object.h" // This is required for internal reflection book keeping.

namespace Engine
{
	using Enum::EnumTemplate;

	Enum_Definition(EnumTemplate,
		Document_Enum("Use this as a template for new enums that will be registered for reflection.");
		
		Document_Item("Example item.");
		Enum_Item(Item1);
		
		Document_Item("Example item.");
		Enum_Item(Item2);
		
		Document_Item("Example item.");
		Enum_Item(Item3);
		
		Document_Item("Example item.");
		Enum_Item(Item4);
		
		Document_Item("Example item.");
		Enum_Item(Item5);
	);
}
