#include "Text.h"

#include "Font.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Appearance.h"
#include "ScreenCanvas.h"

namespace GraphicsEngine
{
	Reflect_Inherited(Text, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(RGBA, TextColor);

		Document("");
		Archivable Class_Member(bool, WrapText);

		Document("");
		Archivable Class_Member(DeviceAxis, LineSpacing);

		Document("");
		Archivable Class_Member(DeviceAxis, FontSize);

		Document("");
		Archivable Class_Member(std::weak_ptr<Font>, FontData);

		Bind_Function(SetText,

			Document("");
			Function_Overload
			(
				Returns_Nothing;
					
				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::string, text);
				);

				Bind_Parameters_No_Return(SetText, text);
			);
		);

		Bind_Function(GetText,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::string);
					
				Overload_Parameters();

				Bind_Parameters(GetText);
			);
		);

		Document("");
		Register_Lua_Property(Contents,
			Property_Getter(GetText, std::string);
			
			Property_Setters(
				Bind_Setter(SetText, std::string);
			);
		);

		Bind_Function(GetTexture,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Texture>);
					
				Overload_Parameters();

				Bind_Parameters(GetTexture);
			);
		);

		Bind_Function(GetBuffer,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<FrameBuffer>);
					
				Overload_Parameters();

				Bind_Parameters(GetBuffer);
			);
		);

		Bind_Function(GetAppearance,

			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Appearance>);
					
				Overload_Parameters();

				Bind_Parameters(GetAppearance);
			);
		);

		Bind_Function(Create,

			Document("");
			Static Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Text>);

				Overload_Parameters
				(
					Document("");
					Function_Parameter(std::shared_ptr<Font>, font);

					Document("");
					Function_Parameter(std::shared_ptr<ScreenCanvas>, parent);

					Document("");
					Function_Parameter(std::string, name);

					Document("");
					Function_Parameter(std::string, text);
				);

				Bind_Parameters(Create, font, parent, name, text);
			);
		);
	);
}