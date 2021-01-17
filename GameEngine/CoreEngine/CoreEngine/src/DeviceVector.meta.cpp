#include "DeviceVector.h"

Reflect_Type(DeviceAxis,
	Document_Class("");

	No_Reference;

	Document("");
	Read_Only Archivable Class_Member(float, Scale);
	
	Document("");
	Read_Only Archivable Class_Member(float, Offset);

	
	Bind_Function(DeviceAxis,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, scale);
			
				Document("");
				Function_Parameter(float, offset);
			);
			
			Bind_Constructor(scale, offset);
		);
	);
	
	Bind_Function(operator-,
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceAxis);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceAxis, other);
			);
			
			Bind_Parameters(operator-, other);
		);
	);
	
	Bind_Function(operator+,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceAxis);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceAxis, other);
			);
			
			Bind_Parameters(operator+, other);
		);
	);
	
	Bind_Function(operator*,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceAxis);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(float, scalar);
			);
			
			Bind_Parameters(operator*, scalar);
		);
	);
	
	Bind_Function(operator/,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceAxis);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(float, scalar);
			);
			
			Bind_Parameters(operator/, scalar);
		);
	);
	
	Bind_Function(operator==,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceAxis, other);
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
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceAxis, other);
			);
			
			Bind_Parameters(operator!=, other);
		);
	);
);

Reflect_Type(DeviceVector,
	Document_Class("");

	No_Reference;

	Document("");
	Read_Only Archivable Class_Member(DeviceAxis, X);
	
	Document("");
	Read_Only Archivable Class_Member(DeviceAxis, Y);

	
	Bind_Function(DeviceVector,
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(float, scaleX);
			
				Document("");
				Function_Parameter(float, offsetX);

				Document("");
				Function_Parameter(float, scaleY);

				Document("");
				Function_Parameter(float, offsetY);
			);
			
			Bind_Constructor(scaleX, offsetX, scaleY, offsetY);
		);
	
		Document("");
		Function_Overload
		(
			Document("");
			Returns_Nothing
	
			Overload_Parameters
			(
				Document("");
				Function_Parameter(DeviceAxis, x);
			
				Document("");
				Function_Parameter(DeviceAxis, y);
			);
			
			Bind_Constructor(x, y);
		);
	);
	
	Bind_Function(operator-,
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceVector);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceVector, other);
			);
			
			Bind_Parameters(operator-, other);
		);
	);
	
	Bind_Function(operator+,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceVector);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceVector, other);
			);
			
			Bind_Parameters(operator+, other);
		);
	);
	
	Bind_Function(operator*,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceVector);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(float, scalar);
			);
			
			Bind_Parameters(operator*, scalar);
		);
	);
	
	Bind_Function(operator/,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(DeviceVector);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(float, scalar);
			);
			
			Bind_Parameters(operator/, scalar);
		);
	);
	
	Bind_Function(operator==,
	
		Document("");
		Function_Overload
		(
			Document("");
			Overload_Returns(bool);
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceVector, other);
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
	
			Overload_Parameters(
				Document("");
				Function_Parameter(DeviceVector, other);
			);
			
			Bind_Parameters(operator!=, other);
		);
	);
);