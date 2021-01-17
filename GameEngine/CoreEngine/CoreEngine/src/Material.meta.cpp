#include "Material.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Material, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(RGBA, Diffuse);

		Document("");
		Archivable Class_Member(RGBA, Specular);

		Document("");
		Archivable Class_Member(RGBA, Ambient);

		Document("");
		Archivable Class_Member(RGBA, Emission);

		Document("");
		Archivable Class_Member(int, Shininess);
	);
}
