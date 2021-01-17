#include "GameWindow.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(GameWindow, Object,
		Document_Class("");
		
		Bind_Function(GetRefreshRate,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);

				Overload_Parameters();

				Bind_Parameters(GetRefreshRate);
			);
		);
		
		Bind_Function(GetResolution,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetResolution);
			);
		);
		
		Bind_Function(SetMousePosition,

			Document("");
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, mousePosition);
				);

				Bind_Parameters_No_Return(SetMousePosition, mousePosition);
			);
		);
	);
}
