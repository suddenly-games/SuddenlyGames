#include "PointGenerator.h"

Reflect_Type(VectorGenerator,
	Document_Class("");

	No_Reference;

	Bind_Function(Generate,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(Vector3);

			Overload_Parameters();

			Bind_Parameters(Generate);
		)
	);
);

Reflect_Type(NumberRange,
	Document_Class("");

	No_Reference;

	Document("");
	Class_Member(float, Min);

	Document("");
	Class_Member(float, Max);

	Bind_Function(rand,

		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(float);

			Overload_Parameters();

			Bind_Parameters(rand);
		)
	);

	Bind_Function(Set,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, min);

				Document("");
				Function_Parameter(float, max);
			);

			Bind_Parameters_No_Return(Set, min, max);
		)
	);

	Bind_Function(NumberRange,

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
				Function_Parameter(float, min);
			);

			Bind_Constructor(min);
		);

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, min);

				Document("");
				Function_Parameter(float, max);
			);

			Bind_Constructor(min, max);
		);
	);
);

Reflect_Type(PointGenerator,
	Document_Class("");

	No_Reference;

	Document("");
	Class_Member(Vector3, Position);

	Bind_Function(PointGenerator,

		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing;

			Overload_Parameters
			(
				Document("");
				Function_Parameter_Default(Vector3, position, Vector3());
			);

			Bind_Constructor(position);
		);
	);
);

Reflect_Type(UnitVectorGenerator,
	Document_Class("");

	No_Reference;
);

Reflect_Type(ConeUnitVectorGenerator,
	Document_Class("");

	No_Reference;

	Document("");
	Class_Member(Vector3, Normal);

	Document("");
	Class_Member(NumberRange, Angle);
);
