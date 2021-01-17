#include "ScreenCanvas.h"

namespace GraphicsEngine
{
	Reflect_Inherited(ScreenCanvas, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Visible);
		
		Document("");
		Archivable Class_Member(std::weak_ptr<GraphicsEngine::Appearance>, Appearance);
	);
}