#include "Texture.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Texture, Object,
		Document_Class("");

		Bind_Function(Load,

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, width);

					Document("");
					Function_Parameter(int, height);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::SampleType>, sampleType, Enum::SampleType::Nearest);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::WrapType>, wrapType, Enum::WrapType::Repeat);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::DataType>, dataType, Enum::DataType::UnsignedByte);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::InternalFormat>, internalFormat, Enum::InternalFormat::RGBA);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::Format>, format, Enum::Format::RGBA);
					
					Document("");
					Function_Parameter_Default(bool, invertedY, false);
				);

				Bind_Parameters_No_Return(Load, width, height, sampleType, wrapType, dataType, internalFormat, format, invertedY);
			);

			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, fileName);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::SampleType>, sampleType, Enum::SampleType::Nearest);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::WrapType>, wrapType, Enum::WrapType::Repeat);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::DataType>, dataType, Enum::DataType::UnsignedByte);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::InternalFormat>, internalFormat, Enum::InternalFormat::RGBA);

					Document("");
					Function_Parameter_Default(LuaEnum<Enum::Format>, format, Enum::Format::RGBA);
					
					Document("");
					Function_Parameter_Default(bool, isDXT5, false);
				);

				Bind_Parameters_No_Return(Load, fileName, sampleType, wrapType, dataType, internalFormat, format, isDXT5);
			);
		);

		Bind_Function(GetWidth,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
		
				Overload_Parameters();
		
				Bind_Parameters(GetWidth);
			);
		);

		Bind_Function(GetHeight,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(int);
		
				Overload_Parameters();
		
				Bind_Parameters(GetHeight);
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

		Bind_Function(GetPixel,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(RGBA);
		
				Overload_Parameters
				(
					Document("");
					Function_Parameter(int, x);

					Document("");
					Function_Parameter(int, y);
				);
		
				Bind_Parameters(GetPixel, x, y);
			);
		);

		Bind_Function(HasInvertedY,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(bool);
		
				Overload_Parameters();
		
				Bind_Parameters(HasInvertedY);
			);
		);
	);
}
