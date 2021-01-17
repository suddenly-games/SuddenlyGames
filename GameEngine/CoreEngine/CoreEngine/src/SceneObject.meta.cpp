#include "Scene.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(SceneObject, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(std::weak_ptr<Material>, MaterialProperties);

		Document("");
		Archivable Class_Member(bool, Visible);
	);
}
