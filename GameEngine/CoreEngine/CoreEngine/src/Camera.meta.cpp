#include "Camera.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Camera, Object,
		Document_Class("");
		
		Document("");
		Archivable Class_Member(Vector3, LightDirection);

		Bind_Function(SetTransformation,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(Matrix3, newTransformation);
				);
			
				Bind_Parameters_No_Return(SetTransformation, newTransformation);
			);
		);

		Bind_Function(SetProperties,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, fieldOfView);

					Document("");
					Function_Parameter(float, aspectRatio);

					Document("");
					Function_Parameter(float, near);

					Document("");
					Function_Parameter(float, far);
				);
			
				Bind_Parameters_No_Return(SetProperties, fieldOfView, aspectRatio, near, far);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, width);

					Document("");
					Function_Parameter(float, height);

					Document("");
					Function_Parameter(float, projectionPlane);

					Document("");
					Function_Parameter(float, near);

					Document("");
					Function_Parameter(float, far);
				);
			
				Bind_Parameters_No_Return(SetProperties, width, height, projectionPlane, near, far);
			);
		);

		Bind_Function(GetTransformation,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetTransformation);
			);
		);

		Bind_Function(GetTransformationInverse,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetTransformationInverse);
			);
		);

		Bind_Function(GetProjectionMatrix,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetProjectionMatrix);
			);
		);

		Bind_Function(GetProjection,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetProjection);
			);
		);

		Bind_Function(GetDimensions,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);
			
				Overload_Parameters();
			
				Bind_Parameters(GetDimensions);
			);
		);

		Bind_Function(GetProjectionPlane,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
			
				Overload_Parameters();
			
				Bind_Parameters(GetProjectionPlane);
			);
		);

		Bind_Function(GetNearPlane,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
			
				Overload_Parameters();
			
				Bind_Parameters(GetNearPlane);
			);
		);

		Bind_Function(GetFarPlane,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
			
				Overload_Parameters();
			
				Bind_Parameters(GetFarPlane);
			);
		);

		Bind_Function(GetAspectRatio,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
			
				Overload_Parameters();
			
				Bind_Parameters(GetAspectRatio);
			);
		);

		Bind_Function(GetFieldOfView,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(float);
			
				Overload_Parameters();
			
				Bind_Parameters(GetFieldOfView);
			);
		);
	);
}
