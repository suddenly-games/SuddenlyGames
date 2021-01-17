#include "CollisionGroup.h"

namespace Engine
{
	using Enum::InteractionType;
	using Physics::CollisionGroup;

	Enum_Definition(InteractionType,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(None);
		
		Document_Item("");
		Enum_Item(Detect);

		Document_Item("");
		Enum_Item(Resolve);
	);

	Reflect_Inherited(CollisionGroup, Object,
		Document_Class("");
	
		Bind_Function(AddInteraction,
	
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Object>, group);

					Document("");
					Function_Parameter(LuaEnum<Enum::InteractionType>, interactionType);
				);

				Bind_Parameters_No_Return(AddInteraction, group, interactionType);
			);
		);
		
		Bind_Function(GetInteraction,
	
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(LuaEnum<Enum::InteractionType>);
	
				Overload_Parameters(
					Document("");
					Function_Parameter(std::shared_ptr<Object>, group);
				);

				Bind_Parameters(GetInteraction, group);
			);
		);
	);
}