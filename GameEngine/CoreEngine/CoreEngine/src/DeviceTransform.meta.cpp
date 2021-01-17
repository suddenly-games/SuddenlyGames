#include "DeviceTransform.h"

namespace Engine
{
	using Enum::Alignment;

	Enum_Definition(Alignment,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(Minimum);
		
		Document_Item("");
		Enum_Item(Center);

		Document_Item("");
		Enum_Item(Maximum);
	);
}

namespace GraphicsEngine
{
	Reflect_Inherited(DeviceTransform, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Enabled);

		Document("");
		Archivable Class_Member(bool, Visible);

		Document("");
		Archivable Class_Member(float, AspectRatio);
		
		Document("");
		Archivable Class_Member(LuaEnum<Enum::Alignment>, AlignX);

		Document("");
		Archivable Class_Member(LuaEnum<Enum::Alignment>, AlignY);

		Document("");
		Archivable Class_Member(DeviceVector, Size);

		Document("");
		Archivable Class_Member(DeviceVector, Position);

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

		Bind_Function(GetInverseTransformation,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Matrix3);

				Overload_Parameters();

				Bind_Parameters(GetInverseTransformation);
			);
		);

		Bind_Function(GetAbsoluteSize,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters();

				Bind_Parameters(GetAbsoluteSize);
			);
		);

		Bind_Function(GetAbsolutePosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters(
					Document("");
					Function_Parameter_Default(DeviceVector, point, DeviceVector(0, 0, 0, 0));
				);

				Bind_Parameters(GetAbsolutePosition, point);
			);
		);

		Bind_Function(GetLocalPosition,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters(
					Document("");
					Function_Parameter(DeviceVector, point);
				);

				Bind_Parameters(GetLocalPosition, point);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(Vector3);

				Overload_Parameters(
					Document("");
					Function_Parameter(Vector3, point);
				);

				Bind_Parameters(GetLocalPosition, point);
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

		Bind_Function(ContainsLocalPoint,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters(
					Document("");
					Function_Parameter(Vector3, point);
				);

				Bind_Parameters(ContainsLocalPoint, point);
			);
		);

		Bind_Function(ContainsPoint,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters(
					Document("");
					Function_Parameter(Vector3, point);
				);

				Bind_Parameters(ContainsPoint, point);
			);
		);

		Bind_Function(HasMoved,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(HasMoved);
			);
		);

		Bind_Function(IsVisible,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(IsVisible);
			);
		);

		Bind_Function(IsEnabled,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);

				Overload_Parameters();

				Bind_Parameters(IsEnabled);
			);
		);

		Bind_Function(UpdateTransformation,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters();

				Bind_Parameters_No_Return(UpdateTransformation);
			);
		);
	);
}