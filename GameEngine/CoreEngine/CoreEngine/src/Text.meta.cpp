#include "Text.h"

#include "Font.h"

namespace GraphicsEngine
{
	Reflect_Inherited(Text, Object,
		Document_Class("");

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
	);
}