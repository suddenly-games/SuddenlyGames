#include "FrameBuffer.h"

#include "Texture.h"

namespace GraphicsEngine
{
	Reflect_Inherited(FrameBuffer, Object,
		Document_Class("");
		
		Document("");
		Static Archivable Class_Member(Dimensions, WindowSize);

		Bind_Function(GetTexture,
		
			Document("");
			Function_Overload
			(
				Document("");
				Overload_Returns(std::shared_ptr<Texture>);
			
				Overload_Parameters
				(
					Document("");
					Function_Parameter_Default(unsigned int, attachmentSlot, 0);
				);
			
				Bind_Parameters(GetTexture, attachmentSlot);
			);
		);
	);
}