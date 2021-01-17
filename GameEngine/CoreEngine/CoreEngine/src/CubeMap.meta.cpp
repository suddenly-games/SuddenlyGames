#include "CubeMap.h"

#include "FrameBuffer.h"

using Engine::Object;

Reflect_Inherited(CubeMapTexture, Object,
	Document_Class("");

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Front);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Back);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Left);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Right);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Top);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::Texture>, Bottom);
);

Reflect_Inherited(CubeMapBuffer, Object,
	Document_Class("");

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Front);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Back);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Left);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Right);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Top);

	Document("");
	Archivable Class_Member(std::weak_ptr<GraphicsEngine::FrameBuffer>, Bottom);
);
