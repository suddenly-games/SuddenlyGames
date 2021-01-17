#include "CanvasStencil.h"

#include "Texture.h"
#include "FrameBuffer.h"

namespace GraphicsEngine
{
	Reflect_Inherited(CanvasStencil, Object,
		Document_Class("");

		Document("");
		Archivable Class_Member(bool, Enabled);

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
	);
}