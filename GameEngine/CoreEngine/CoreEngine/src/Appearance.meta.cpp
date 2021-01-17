#include "Appearance.h"

namespace GraphicsEngine
{
	Reflect_Inherited(Appearance, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, BlendTexture);

		Document("");
		Archivable Class_Member(RGBA, Color);

		Document("");
		Archivable Class_Member(RGBA, TextureColor);

		Document("");
		Archivable Class_Member(Vector3, UVScale);

		Document("");
		Archivable Class_Member(Vector3, UVOffset);

		Document("");
		Archivable Class_Member(std::shared_ptr<GraphicsEngine::Texture>, Texture);
	);
}