#include "ScreenCanvas.h"

#include "DeviceTransform.h"
#include "Graphics.h"
#include "CanvasStencil.h"

namespace GraphicsEngine
{
	void ScreenCanvas::Draw()
	{
		if (!Visible)
			return;

		std::shared_ptr<DeviceTransform> transform = GetComponent<DeviceTransform>();

		if (transform == nullptr)
			return;

		if (ShaderProgram::GetCurrentProgram() == Programs::Screen)
			DrawScreen(transform);
	}

	void ScreenCanvas::DrawScreen(const std::shared_ptr<DeviceTransform>& transform)
	{
		Matrix3 transformation = transform->GetTransformation();
		Programs::Screen->transform.Set(transformation);
		Programs::Screen->resolution.Set(transform->GetResolution());

		std::shared_ptr<CanvasStencil> stencil = GetComponent<CanvasStencil>();

		if (stencil != nullptr && stencil->Enabled)
			Programs::Screen->SetClippingMask(stencil->GetTexture(), 1);

		if (Appearance.expired())
		{
			Programs::Screen->SetTexture(nullptr);
			Programs::Screen->blendTexture.Set(false);

			Programs::Screen->color.Set(RGBA());
			Programs::Screen->textureColor.Set(RGBA());

			Programs::Screen->uvScale.Set(Vector3(1, 1));
			Programs::Screen->uvOffset.Set(Vector3());
		}
		else
		{
			auto appearance = Appearance.lock();

			Programs::Screen->SetTexture(appearance->Texture.lock());
			Programs::Screen->blendTexture.Set(appearance->BlendTexture);

			Programs::Screen->color.Set(appearance->Color);
			Programs::Screen->textureColor.Set(appearance->TextureColor);

			Programs::Screen->uvScale.Set(appearance->UVScale);
			Programs::Screen->uvOffset.Set(appearance->UVOffset);
		}

		Programs::Screen->CoreMeshes.Square->Draw();
	}
}