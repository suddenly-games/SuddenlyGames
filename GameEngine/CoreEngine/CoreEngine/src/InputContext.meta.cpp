#include "InputContext.h"

#include "LuaInput.h"

namespace GraphicsEngine
{
	Reflect_Inherited(InputContext, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Enabled);

		Document("");
		Archivable Class_Member(std::weak_ptr<DeviceTransform>, Device);

		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::UserInput>, InputSource);
	);
}