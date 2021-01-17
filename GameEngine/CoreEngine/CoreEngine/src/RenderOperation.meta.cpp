#include "RenderOperation.h"

namespace GraphicsEngine
{
	Reflect_Inherited(RenderOperation, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(bool, RenderAutomatically);
	);
}
