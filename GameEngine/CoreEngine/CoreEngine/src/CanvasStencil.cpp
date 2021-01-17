#include "CanvasStencil.h"

#include "DeviceTransform.h"
#include "Graphics.h"
#include "FrameBuffer.h"
#include "Texture.h"

namespace GraphicsEngine
{
	CanvasStencil::~CanvasStencil()
	{

	}

	void CanvasStencil::Initialize()
	{
		auto stencilTexture = Engine::Create<Texture>();
		auto stencilBuffer = FrameBuffer::Create(0, 0, StencilTexture.lock(), true);

		StencilTexture = stencilTexture;
		StencilBuffer = stencilBuffer;
		
		stencilTexture->Name = "StencilTexture";
		stencilBuffer->Name = "StencilBuffer";

		stencilBuffer->SetParent(This.lock());
		stencilTexture->SetParent(stencilBuffer);
	}

	void CanvasStencil::Update(float)
	{

	}

	void CanvasStencil::Draw()
	{
		if (!Enabled)
			return;

		std::shared_ptr<DeviceTransform> transform = GetComponent<DeviceTransform>();

		if (transform == nullptr)
			return;

		std::shared_ptr<CanvasStencil> parent = GetComponent<CanvasStencil>();

		Dimensions resolution = transform->GetResolution();

		auto stencilBuffer = StencilBuffer.lock();

		if (stencilBuffer->GetSize() != resolution)
			stencilBuffer->Resize(resolution, true);

		stencilBuffer->DrawTo();

		Graphics::SetClearColor(RGBA(0x00000000));
		Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Programs::ClippingMask->transform.Set(transform->GetTransformation());
		Programs::ClippingMask->hasParent.Set(parent != nullptr && parent->Enabled);

		if (parent != nullptr && parent->Enabled)
			Programs::ClippingMask->textureData.Set(parent->GetTexture());

		Programs::ClippingMask->CoreMeshes.Square->Draw();

		stencilBuffer->Detatch();
	}

	std::shared_ptr<Texture> CanvasStencil::GetTexture()
	{
		return StencilTexture.lock();
	}
}