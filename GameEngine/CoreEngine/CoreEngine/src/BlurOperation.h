#pragma once

#include "Vector3.h"
#include "RenderOperation.h"
#include "Texture.h"
#include "Mesh.h"
#include "FrameBuffer.h"

namespace GraphicsEngine
{
	class BlurPassOperation : public RenderOperation
	{
	public:
		Vector3 Direction;
		std::weak_ptr<Texture> Input;

		void Initialize() {}
		void Update(float) {}

		void Configure(const Vector3& direction, const std::shared_ptr<Texture>& input);

		void Render();

		Instantiable;

		Inherits_Class(RenderOperation);

		Reflected(BlurPassOperation);
	};
}

namespace Engine
{
	////Class_Inherits(GraphicsEngine::BlurPassOperation, GraphicsEngine::RenderOperation);
}

namespace GraphicsEngine
{
	class BlurOperation : public RenderOperation
	{
	public:
		int Radius = 1;
		float Sigma = 1;
		Vector3 Resolution;
		Matrix3 Transform;
		std::weak_ptr<Texture> Input;
		std::weak_ptr<FrameBuffer> Output;
		bool Detatch = true;

		void Initialize();
		void Update(float) {}

		void Configure(int width, int height, GLint sampleType = GL_LINEAR, GLint wrapType = GL_CLAMP_TO_EDGE, GLenum dataType = GL_UNSIGNED_BYTE, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA);

		void SetInput(const std::shared_ptr<Texture>& texture);
		void Render();
		std::shared_ptr<FrameBuffer> GetHorizontalPass() const;

	private:
		std::weak_ptr<FrameBuffer> HorizontalBuffer;
		std::weak_ptr<BlurPassOperation> HorizontalPass;
		std::weak_ptr<BlurPassOperation> VerticalPass;

		Instantiable;

		Inherits_Class(RenderOperation);

		Reflected(BlurOperation);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::BlurOperation, GraphicsEngine::RenderOperation);
}
