#include "Matrix3.h"

Reflect_Type(Matrix3,
	Document_Class(
		"A 3x3 matrix with an extra axis in each direction for affine transformations. "
	);

	No_Reference;

	Bind_Function(Matrix3,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters();

			Bind_Constructor();
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
		
			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(float, x, 0);
		
				Document("");
				Function_Parameter_Default(float, y, 0);
		
				Document("");
				Function_Parameter_Default(float, z, 0);
			);
		
			Bind_Constructor(x, y, z);
		);
		
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
		
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, vector);
			);
		
			Bind_Constructor(vector);
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, position);

				Document("");
				Function_Parameter(Vector3, right);

				Document("");
				Function_Parameter(Vector3, up);

				Document("");
				Function_Parameter(Vector3, front);
			);

			Bind_Constructor(position, right, up, front);
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, position);

				Document("");
				Function_Parameter(Vector3, direction);

				Document("");
				Function_Parameter_Default(Vector3, globalUp, Vector3(0, 1, 0));
			);

			Bind_Constructor(position, direction, globalUp);
		);
	);
	
	Bind_Function(Transposed,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters();

			Bind_Parameters(Transposed);
		);
	);
	
	Bind_Function(Inverted,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters();

			Bind_Parameters(Inverted);
		);
	);
	
	Bind_Function(Rotation,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(Vector3, newTranslation, Vector3());
			);

			Bind_Parameters(Rotation, newTranslation);
		);
	);
	
	Bind_Function(TransformedAround,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, point);
			);

			Bind_Parameters(TransformedAround, point);
		);
	);
	
	Bind_Function(RightVector,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(RightVector);
		);
	);
	
	Bind_Function(UpVector,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(UpVector);
		);
	);
	
	Bind_Function(FrontVector,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(FrontVector);
		);
	);
	
	Bind_Function(Translation,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(Translation);
		);
	);
	
	Bind_Function(ExtractScale,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(ExtractScale);
		);
	);
	
	Bind_Function(Det,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(float);

			Overload_Parameters();

			Bind_Parameters(Det);
		);
	);
	
	Bind_Function(operator+,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Matrix3, other);
			);
	
			Bind_Parameters(operator+, other);
		);
	);
	
	Bind_Function(operator-,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);
		
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Matrix3, other);
			);
		
			Bind_Parameters(operator-, other);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters();

			Bind_Parameters(operator-);
		);
	);
	
	Bind_Function(operator*,
		
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);
		
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Matrix3, other);
			);
		
			Bind_Parameters(operator*, other);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, other);
			);

			Bind_Parameters(operator*, other);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, other);
			);

			Bind_Parameters(operator*, other);
		);
	);
	
	Bind_Function(NewScale,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, x);

				Document("");
				Function_Parameter(float, y);

				Document("");
				Function_Parameter(float, z);
			);

			Static_Bind_Parameters(NewScale, x, y, z);
		);

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, vector);
			);

			Static_Bind_Parameters(NewScale, vector);
		);
	);
	
	Bind_Function(AxisRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, axis);

				Document("");
				Function_Parameter(float, theta);
			);

			Static_Bind_Parameters(AxisRotation, axis, theta);
		);
	);
	
	Bind_Function(PitchRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, theta);
			);

			Static_Bind_Parameters(PitchRotation, theta);
		);
	);
	
	Bind_Function(YawRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, theta);
			);

			Static_Bind_Parameters(YawRotation, theta);
		);
	);
	
	Bind_Function(RollRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, theta);
			);

			Static_Bind_Parameters(RollRotation, theta);
		);
	);
	
	Bind_Function(EulerAnglesRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, pitch);

				Document("");
				Function_Parameter(float, yaw);

				Document("");
				Function_Parameter(float, roll);
			);

			Static_Bind_Parameters(EulerAnglesRotation, pitch, yaw, roll);
		);
	);
	
	Bind_Function(EulerAnglesYawRotation,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, yaw);

				Document("");
				Function_Parameter(float, pitch);

				Document("");
				Function_Parameter(float, roll);
			);

			Static_Bind_Parameters(EulerAnglesRotation, yaw, pitch, roll);
		);
	);
	
	Bind_Function(NewProjection,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, distance);

				Document("");
				Function_Parameter(float, near);

				Document("");
				Function_Parameter(float, far);

				Document("");
				Function_Parameter(float, width);

				Document("");
				Function_Parameter(float, height);
			);

			Static_Bind_Parameters(NewProjection, distance, near, far, width, height);
		);
	);
	
	Bind_Function(Facing,

		Document("");
		Static Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Vector3, position);

				Document("");
				Function_Parameter(Vector3, direction);

				Document("");
				Function_Parameter_Default(Vector3, globalUp, Vector3(0, 1, 0));
			);

			Static_Bind_Parameters(Facing, position, direction, globalUp);
		);
	);
	
	Bind_Function(operator string,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(std::string);

			Overload_Parameters();

			Bind_Parameters(operator std::string);
		);
	);
);