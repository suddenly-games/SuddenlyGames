#include "Light.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Light, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(bool, Enabled);

		Document("");
		Archivable Class_Member(bool, ShadowDebugView);

		Document("");
		Archivable Class_Member(float, Brightness);

		Document("");
		Archivable Class_Member(Vector3, Attenuation);

		Document("");
		Archivable Class_Member(Vector3, Position);

		Document("");
		Archivable Class_Member(Vector3, Direction);

		Document("");
		Archivable Class_Member(RGBA, Diffuse);

		Document("");
		Archivable Class_Member(RGBA, Specular);

		Document("");
		Archivable Class_Member(RGBA, Ambient);

		Document("");
		Archivable Class_Member(float, InnerRadius);

		Document("");
		Archivable Class_Member(float, OuterRadius);

		Document("");
		Archivable Class_Member(int, SpotlightFalloff);

		Document("");
		Archivable Class_Member(LuaEnum<Enum::LightType>, Type);

		Bind_Function(SetShadowsEnabled,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(bool, enabled);

					Document("");
					Function_Parameter_Default(int, width, 128);

					Document("");
					Function_Parameter_Default(int, height, 128);
				);

				Bind_Parameters_No_Return(SetShadowsEnabled, enabled, width, height);
			);
		);

		Bind_Function(AreShadowsEnabled,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(AreShadowsEnabled);
			);
		);
	);
}
