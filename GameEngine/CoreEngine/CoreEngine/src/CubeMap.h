#pragma once

#include "Object.h"

struct CubeSideEnum
{
	enum CubeSide
	{
		Front,
		Back,
		Left,
		Right,
		Top,
		Bottom
	};
};

namespace Enum
{
	typedef CubeSideEnum::CubeSide CubeSide;
}

namespace GraphicsEngine
{
	class FrameBuffer;
	class Texture;
}

class CubeMapTexture : public Engine::Object
{
public:
	void Initialize() {}
	void Update(float) {}

	std::weak_ptr<GraphicsEngine::Texture> Front;
	std::weak_ptr<GraphicsEngine::Texture> Back;
	std::weak_ptr<GraphicsEngine::Texture> Left;
	std::weak_ptr<GraphicsEngine::Texture> Right;
	std::weak_ptr<GraphicsEngine::Texture> Top;
	std::weak_ptr<GraphicsEngine::Texture> Bottom;

	std::shared_ptr<GraphicsEngine::Texture> Get(Enum::CubeSide side) const
	{
		return (&Front)[int(side)].lock();
	}

	std::shared_ptr<GraphicsEngine::Texture> Get(int side) const
	{
		return (&Front)[side].lock();
	}

	Instantiable;

	Inherits_Class(Object);

	Reflected(CubeMapTexture);
};

class CubeMapBuffer : public Engine::Object
{
public:
	void Initialize() {}
	void Update(float) {}

	std::weak_ptr<GraphicsEngine::FrameBuffer> Front;
	std::weak_ptr<GraphicsEngine::FrameBuffer> Back;
	std::weak_ptr<GraphicsEngine::FrameBuffer> Left;
	std::weak_ptr<GraphicsEngine::FrameBuffer> Right;
	std::weak_ptr<GraphicsEngine::FrameBuffer> Top;
	std::weak_ptr<GraphicsEngine::FrameBuffer> Bottom;

	std::shared_ptr<GraphicsEngine::FrameBuffer> Get(Enum::CubeSide side) const
	{
		return (&Front)[int(side)].lock();
	}

	std::shared_ptr<GraphicsEngine::FrameBuffer> Get(int side) const
	{
		return (&Front)[side].lock();
	}

	Instantiable;

	Inherits_Class(Object);

	Reflected(CubeMapBuffer);
};

namespace Engine
{
	//Class_Inherits(CubeMapTexture, Engine::Object);
	//Class_Inherits(CubeMapBuffer, Engine::Object);
}
