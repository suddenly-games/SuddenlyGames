#include "InterfaceDrawOperation.h"

namespace GraphicsEngine
{
	Reflect_Inherited(InterfaceDrawOperation, RenderOperation,
		Document_Class("");

		Document("");
		Archivable Class_Member(std::weak_ptr<DeviceTransform>, CurrentScreen);
	);
}