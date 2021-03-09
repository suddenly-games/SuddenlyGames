#include "Button.h"

namespace GraphicsEngine
{
	Reflect_Inherited(Button, Object,
		Document_Class("");

		Document("");
		Class_Member(std::shared_ptr<Appearance>, Hover);

		Document("");
		Class_Member(std::shared_ptr<Appearance>, Pressed);

		Document("");
		Class_Member(std::shared_ptr<Appearance>, Idle);

		Document("");
		Class_Member(std::weak_ptr<InputSubscription>, Binding);

		Document("");
		Class_Member(LuaEnum<Enum::BoundDevice>, Device);
	);
}