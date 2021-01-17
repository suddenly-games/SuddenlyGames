#include "Textures.h"

#include "Texture.h"

namespace GraphicsEngine
{
	using Engine::Object;

	Reflect_Inherited(Textures, Object,
		Document_Class("");

		Bind_Function(Create,

			Document("");
			Static Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Texture>);

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

				Bind_Parameters(Create, width, height, sampleType, wrapType, dataType, internalFormat, format, invertedY);
			);

			Document("");
			Function_Overload
			(
				Document("");
			Overload_Returns(std::shared_ptr<Texture>);

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
				);

				Bind_Parameters(Create, fileName, sampleType, wrapType, dataType, internalFormat, format);
			);
		);

		Bind_Function(Add,
		
			Document("");
			Function_Overload
			(
				Document("");
				Returns_Nothing;
		
				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<Texture>, texture);

					Document("");
					Function_Parameter(std::string, name);
				);
		
				Bind_Parameters_No_Return(Add, texture, name);
			);
		);
	);
}
