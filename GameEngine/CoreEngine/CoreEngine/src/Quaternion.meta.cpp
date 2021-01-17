#include "Quaternion.h"

Reflect_Type(Quaternion,
	Document_Class(
		""
	);

	No_Reference;
	
	Document("");
	Read_Only Archivable Class_Member(float, W);

	Document("");
	Read_Only Archivable Class_Member(float, X);
	
	Document("");
	Read_Only Archivable Class_Member(float, Y);
	
	Document("");
	Read_Only Archivable Class_Member(float, Z);
	
	Bind_Function(Quaternion,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(float, w, 0);
			
				Document("");
				Function_Parameter_Default(float, x, 0);
			
				Document("");
				Function_Parameter_Default(float, y, 0);
			
				Document("");
				Function_Parameter_Default(float, z, 0);
			);
			
			Bind_Constructor(w, x, y, z);
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
				Function_Parameter(Vector3, axis);

				Document("");
				Function_Parameter(float, angle);
			);
			
			Bind_Constructor(axis, angle);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, w);
			
				Document("");
				Function_Parameter(Vector3, vector);
			);
			
			Bind_Constructor(w, vector);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(Matrix3, matrix);
			);
			
			Bind_Constructor(matrix);
		);
	);

	Bind_Function(Invert,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters();

			Bind_Parameters(Invert);
		);
	);

	Bind_Function(Slerp,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, destination);

				Document("");
				Function_Parameter(float, t);
			);

			Bind_Parameters(Slerp, destination, t);
		);
	);

	Bind_Function(Matrix,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Matrix3);

			Overload_Parameters();

			Bind_Parameters(Matrix);
		);
	);

	Bind_Function(Vector,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(operator Vector3);
		);
	);

	Bind_Function(operator*,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, other);
			);

			Bind_Parameters(operator*, other);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, scalar);
			);

			Bind_Parameters(operator*, scalar);
		);
	);

	Bind_Function(operator-,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters();

			Bind_Parameters(operator-);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, other);
			);

			Bind_Parameters(operator-, other);
		);
	);

	Bind_Function(operator+,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Quaternion);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, other);
			);

			Bind_Parameters(operator+, other);
		);
	);

	Bind_Function(operator==,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, other);
			);

			Bind_Parameters(operator==, other);
		);
	);

	Bind_Function(operator!=,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);

			Overload_Parameters
			(
				Document("");
				Function_Parameter(Quaternion, other);
			);

			Bind_Parameters(operator!=, other);
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
