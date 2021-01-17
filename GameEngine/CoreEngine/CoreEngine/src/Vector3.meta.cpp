#include "Vector3.h"

Reflect_Type(Vector3,
	Document_Class(
		"A 3 dimensional vector < x, y, z > with an extra w axis to make it compatible "
		"with affine matrix transformations. Comes with a set of built in vector math "
		"operations such as arithmetic, normalization, dot, and cross products."
	);

	No_Reference;

	Document("The vector's x axis, the first axis. This axis represents left and right.");
	Read_Only Archivable Class_Member(float, X);
	
	Document("The vector's y axis, the second axis. This axis represents up and down.");
	Read_Only Archivable Class_Member(float, Y);
	
	Document("The vector's z axis, the third axis. This axis represents forward n backward.");
	Read_Only Archivable Class_Member(float, Z);
	
	Document(
		"The vector's w axis, the fourth axis. This axis affects translation matrices.\n"
		"A value of 0 means it behaves like a vector; it will not be translated by "
		"matrices.\n"
		"A value of 1 means it behaves like a point; it will be translated by matrices.\n"
		"Any other value means there might have been an arithmetic error or it is in "
		"homogeneous coordinate space, which may be a result of a perspective projection."
		" Divide the vector by w to convert it back into a point."
	);
	Read_Only Archivable Class_Member(float, W);
	
	Bind_Function(Vector3,
	
		Document("Constructs a new 3D vector with given coordinates.");
		Function_Overload
		(
			Document("The newly constructed 3D vector.");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(float, x, 0);

				Document("");
				Function_Parameter_Default(float, y, 0);

				Document("");
				Function_Parameter_Default(float, z, 0);

				Document("");
				Function_Parameter_Default(float, w, 0);
			);

			Bind_Constructor(x, y, z, w);
		);
	);
	
	Bind_Function(Dot,
	
		Document("Computes the dot product between two vectors.");
		Function_Overload
		(
			Document("The scalar result of the dot product.");
			Overload_Returns(float);
	
			Overload_Parameters(
				Document("The vector to be dot-ed with.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(Dot, other);
		);
	);
	
	Bind_Function(Cross,
	
		Document("Computes the cross product between two vectors.");
		Function_Overload
		(
			Document("A vector that is perpendicular to both input vectors.");
			Overload_Returns(Vector3);
	
			Overload_Parameters(
				Document("The vector to be dot-ed with.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(Cross, other);
		);
	);

	Bind_Function(Unit,

		Document("Generates a copy of this vector, but with a unit length of 1.");
		Function_Overload
		(
			Document("The unit vector that represents this vector's direction.");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(Unit);
		);
	);

	Bind_Function(Length,

		Document("Calculates the length of this vector.");
		Function_Overload
		(
			Document("The scalar length of this vector.");
			Overload_Returns(float);

			Overload_Parameters();

			Bind_Parameters(Length);
		);
	);

	Bind_Function(SquareLength,

		Document("Calculates the length of this vector squared.");
		Function_Overload
		(
			Document("The scalar length squared of this vector.");
			Overload_Returns(float);

			Overload_Parameters();

			Bind_Parameters(SquareLength);
		);
	);
	
	Bind_Function(operator-,
	
		Document("Creates a copy of this vector with the direction flipped.");
		Function_Overload
		(
			Document("A flipped version of this vector.");
			Overload_Returns(Vector3);
	
			Overload_Parameters();
			
			Bind_Parameters(Negate);
		);

		Document("Subtracts a vector from this one component-wise and returns the result.");
		Function_Overload
		(
			Document("The subtraction between the two vectors.");
			Overload_Returns(Vector3);
	
			Overload_Parameters(
				Document("The vector to subtract.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(operator-, other);
		);
	);
	
	Bind_Function(operator+,
	
		Document("Adds two vectors together component-wise and returns the result.");
		Function_Overload
		(
			Document("The addition between the two vectors");
			Overload_Returns(Vector3);
	
			Overload_Parameters(
				Document("The vector to be added with.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(operator+, other);
		);
	);
	
	Bind_Function(operator*,
	
		Document("Calculates a copy of this vector that is resized by a scalar factor.");
		Function_Overload
		(
			Document("The scaled vector.");
			Overload_Returns(Vector3);
	
			Overload_Parameters(
				Document("The scalar to scale the new vector with.");
				Function_Parameter(float, scalar);
			);
			
			Bind_Parameters(operator*, scalar);
		);

		Document("Computes the dot product between two vectors.");
		Function_Overload
		(
			Document("The scalar result of the dot product.");
			Overload_Returns(float);
	
			Overload_Parameters(
				Document("The vector to be dot-ed with.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(operator*, other);
		);
	);
	
	Bind_Function(operator==,
	
		Document(
			"Calculates a component-wise comparison of equality with an another vector using "
			"epsilon degree of error to account for floating point drift."
		);
		Function_Overload
		(
			Document("True if the vectors are close enough to being equal, false otherwise.");
			Overload_Returns(bool);
	
			Overload_Parameters(
				Document("The vector to be compared with.");
				Function_Parameter(Vector3, other);
			);
			
			Bind_Parameters(operator==, other);
		);
	);
	
	Bind_Function(operator!=,
	
		Document(
			"Calculates a component-wise comparison of inequality with an another vector "
			"using epsilon degree of error to account for floating point drift."
		);
		Function_Overload
		(
			Document("False if the vectors are close enough to being equal, true otherwise.");
			Overload_Returns(bool);
	
			Overload_Parameters(
				Document("The vector to be compared with.");
				Function_Parameter(Vector3, other);
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
