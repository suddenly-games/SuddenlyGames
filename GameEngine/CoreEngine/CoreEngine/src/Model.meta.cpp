#include "Model.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Model, SceneObject,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(std::weak_ptr<Engine::ModelAsset>, Asset);

		Document("");
		Archivable Class_Member(bool, BlendTexture);

		Document("");
		Archivable Class_Member(bool, CubeMapped);

		Document("");
		Archivable Class_Member(bool, FlipCubeMapV);

		Document("");
		Archivable Class_Member(bool, CompressedNormalMap);

		Document("");
		Archivable Class_Member(bool, UseOffsetMap);

		Document("");
		Archivable Class_Member(RGBA, Color);

		Document("");
		Archivable Class_Member(RGBA, GlowColor);

		Document("");
		Archivable Class_Member(RGBA, TextureColor);

		Document("");
		Archivable Class_Member(Vector3, UVScale);

		Document("");
		Archivable Class_Member(Vector3, UVOffset);

		Document("");
		Archivable Class_Member(Vector3, BoxScale);

		Document("");
		Archivable Class_Member(std::weak_ptr<Texture>, DiffuseTexture);

		Document("");
		Archivable Class_Member(std::weak_ptr<Texture>, NormalMap);

		Document("");
		Archivable Class_Member(std::weak_ptr<Texture>, SpecularMap);

		Bind_Function(GetBoundingBox,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Aabb);
			
				Overload_Parameters();
			
				Bind_Parameters(GetBoundingBox);
			);
		);
	);
}
