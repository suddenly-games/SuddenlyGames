#include "DebugDraw.h"

#include "Model.h"
#include "Transform.h"

using Engine::Object;

Reflect_Inherited(DebugDraw, Object,
	Document_Class("");
	
	Class_Member(std::weak_ptr<Engine::ModelAsset>, LineAsset);
	Class_Member(std::weak_ptr<Engine::ModelAsset>, TriangleAsset);
	Class_Member(std::weak_ptr<GraphicsEngine::Material>, MaterialProperties);

	Bind_Function(GetScene,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(std::shared_ptr<GraphicsEngine::Scene>);
	
			Overload_Parameters();

			Bind_Parameters(GetScene);
		);
	);

	Bind_Function(Reset,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters();

			Bind_Parameters_No_Return(Reset);
		);
	);

	Bind_Function(Clear,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters();

			Bind_Parameters_No_Return(Clear);
		);
	);

	Bind_Function(DrawLine,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, point1);

				Document("");
				Function_Parameter(Vector3, point2);

				Document("");
				Function_Parameter_Default(RGBA, color, 0xFFFFFFFF);

				Document("");
				Function_Parameter_Default(float, thickness, 0.1f);

				Document("");
				Function_Parameter_Default(bool, glows, false);
			);

			Bind_Parameters_No_Return(DrawLine, point1, point2, color, thickness, glows);
		);
	);

	Bind_Function(DrawTriangle,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, point1);

				Document("");
				Function_Parameter(Vector3, point2);

				Document("");
				Function_Parameter(Vector3, point3);

				Document("");
				Function_Parameter_Default(RGBA, color, 0xFFFFFFFF);

				Document("");
				Function_Parameter_Default(float, thickness, 0.1f);

				Document("");
				Function_Parameter_Default(bool, glows, false);
			);

			Bind_Parameters_No_Return(DrawTriangle, point1, point2, point3, color, thickness, glows);
		);
	);
);