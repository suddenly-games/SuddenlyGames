#include "Transform.h"

namespace Engine
{
	Reflect_Inherited(Transform, Object,
		Document_Class(
			"This type is a general purpose position and orientation storage + tracking "
			"class. It is designed to be a useful reference point for any system or object "
			"that needs to track location information."
		);

		Document("");
		Archivable Class_Member(bool, IsStatic);
	
		Document("");
		Archivable Class_Member(Matrix3, Transformation);

		Document("");
		Archivable Class_Member(bool, InheritTransformation);

		Bind_Function(Update,
			Document("Recalculates the world transformation based on the ancestor's transformation.");
			Function_Overload
			(
				Returns_Nothing;

				Overload_Parameters
				(
					Document("The elapsed time that the object should use. This parameter isn't used.");
					Function_Parameter(float, delta);
				);

				Bind_Parameters_No_Return(Update, delta);
			);
		);

		Bind_Function(GetPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetPosition);
			);
		);

		Bind_Function(SetPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, position);
				);

				Bind_Parameters_No_Return(SetPosition, position);
			);
		);

		Bind_Function(Move,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, offset);
				);

				Bind_Parameters_No_Return(Move, offset);
			);
		);

		Bind_Function(GetWorldPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetWorldPosition);
			);
		);

		Bind_Function(GetWorldTransformation,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);

				Overload_Parameters();

				Bind_Parameters(GetWorldTransformation);
			);
		);

		Bind_Function(GetWorldTransformationInverse,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);

				Overload_Parameters();

				Bind_Parameters(GetWorldTransformationInverse);
			);
		);

		Bind_Function(GetWorldOrientation,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Quaternion);

				Overload_Parameters();

				Bind_Parameters(GetWorldOrientation);
			);
		);

		Bind_Function(GetOrientation,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Quaternion);

				Overload_Parameters();

				Bind_Parameters(GetOrientation);
			);
		);

		Bind_Function(SetOrientation,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Quaternion, orientation);
				);

				Bind_Parameters_No_Return(SetOrientation, orientation);
			);
		);

		Bind_Function(Rotate,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Quaternion, rotation);
				);

				Bind_Parameters_No_Return(Rotate, rotation);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, axis);

					Document("");
					Function_Parameter(float, angle);
				);

				Bind_Parameters_No_Return(Rotate, axis, angle);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, angles);
				);

				Bind_Parameters_No_Return(Rotate, angles);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
				
				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, pitch);

					Document("");
					Function_Parameter(float, roll);

					Document("");
					Function_Parameter(float, yaw);
				);

				Bind_Parameters_No_Return(Rotate, pitch, roll, yaw);
			);
		);

		Bind_Function(GetEulerAngles,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetEulerAngles);
			);
		);

		Bind_Function(SetEulerAngles,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, pitch);

					Document("");
					Function_Parameter(float, roll);

					Document("");
					Function_Parameter(float, yaw);
				);

				Bind_Parameters_No_Return(SetEulerAngles, pitch, roll, yaw);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, angles);
				);

				Bind_Parameters_No_Return(SetEulerAngles, angles);
			);
		);

		Bind_Function(GetEulerAnglesYaw,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetEulerAnglesYaw);
			);
		);

		Bind_Function(SetEulerAnglesYaw,
			
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, angles);
				);

				Bind_Parameters_No_Return(SetEulerAnglesYaw, angles);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, yaw);

					Document("");
					Function_Parameter(float, pitch);

					Document("");
					Function_Parameter(float, roll);
				);

				Bind_Parameters_No_Return(SetEulerAnglesYaw, yaw, pitch, roll);
			);
		);

		Bind_Function(RotateYaw,
			
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, angles);
				);

				Bind_Parameters_No_Return(RotateYaw, angles);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
				
				Overload_Parameters
				(
					Document("");
					Function_Parameter(float, yaw);

					Document("");
					Function_Parameter(float, pitch);

					Document("");
					Function_Parameter(float, roll);
				);

				Bind_Parameters_No_Return(RotateYaw, yaw, pitch, roll);
			);
		);

		Bind_Function(GetScale,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetScale);
			);
		);

		Bind_Function(SetScale,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, scale);
				);

				Bind_Parameters_No_Return(SetScale, scale);
			);
		);

		Bind_Function(Rescale,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Vector3, scale);
				);

				Bind_Parameters_No_Return(Rescale, scale);
			);
		);

		Bind_Function(TransformBy,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Matrix3, transformation);
				);

				Bind_Parameters_No_Return(TransformBy, transformation);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Quaternion, transformation);

					Document("");
					Function_Parameter_Default(Vector3, point, Vector3());
				);

				Bind_Parameters_No_Return(TransformBy, transformation, point);
			);
		);

		Bind_Function(TransformByRelative,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Matrix3, transformation);
				);

				Bind_Parameters_No_Return(TransformByRelative, transformation);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(Quaternion, transformation);

					Document("");
					Function_Parameter_Default(Vector3, point, Vector3());
				);

				Bind_Parameters_No_Return(TransformByRelative, transformation, point);
			);
		);
	);
}
