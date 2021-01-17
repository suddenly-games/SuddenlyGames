#include "Texture.h"

namespace Engine
{
	using Enum::SampleType;
	using Enum::WrapType;
	using Enum::InternalFormat;
	using Enum::Format;

	Enum_Definition(SampleType,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(Nearest);
		
		Document_Item("");
		Enum_Item(Linear);
	);

	Enum_Definition(WrapType,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(Repeat);

		Document_Item("");
		Enum_Item(ClampCutOff);

		Document_Item("");
		Enum_Item(ClampExtend);
	);

	Enum_Definition(InternalFormat,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(RGBA);
	);

	Enum_Definition(Format,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(RGBA);
	);

	Enum_Definition(DataType,
		Document_Enum("");
		
		Document_Item("");
		Enum_Item(UnsignedByte);

		Document_Item("");
		Enum_Item(Float);
	);
}