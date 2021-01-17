#include "BlurOperation.h"

#include "Graphics.h"
#include "Textures.h"

namespace GraphicsEngine
{
	void BlurPassOperation::Configure(const Vector3& direction, const std::shared_ptr<Texture>& input)
	{
		Direction = direction;
		Input = input;
	}

	void BlurPassOperation::Render()
	{
		Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Programs::Blur->blurDirection.Set(Direction);
		Programs::Blur->textureData.Set(Input.lock());

		Programs::Blur->CoreMeshes.Square->Draw();
	}

	void BlurOperation::Initialize()
	{
		auto horizontalPass = Engine::Create<BlurPassOperation>();
		auto verticalPass = Engine::Create<BlurPassOperation>();

		horizontalPass->SetParent(This.lock());
		verticalPass->SetParent(This.lock());

		HorizontalPass = horizontalPass;
		VerticalPass = verticalPass;
	}

	void BlurOperation::Configure(int width, int height, GLint sampleType, GLint wrapType, GLenum dataType, GLint internalFormat, GLenum format)
	{
		auto bufferTexture = Textures::Create(width, height, sampleType, wrapType, dataType, internalFormat, format);
		auto horizontalBuffer = FrameBuffer::Create(width, height, bufferTexture, false, false);

		horizontalBuffer->SetParent(This.lock());
		bufferTexture->SetParent(horizontalBuffer);

		HorizontalBuffer = horizontalBuffer;
		HorizontalPass.lock()->Configure(Vector3(1, 0), Input.lock());
		VerticalPass.lock()->Configure(Vector3(0, 1), HorizontalBuffer.lock()->GetTexture());
	}

	void BlurOperation::Render()
	{
		Programs::Blur->Use();
		Programs::Blur->SetBlurRadius(Radius, Sigma);
		Programs::Blur->resolution.Set(Resolution);
		Programs::Blur->transform.Set(Transform);

		auto horizontalBuffer = HorizontalBuffer.lock();
		auto horizontalPass = HorizontalPass.lock();
		auto output = Output.lock();

		HorizontalBuffer.lock()->DrawTo();

		horizontalPass->Render();

		if (output != nullptr)
			output->DrawTo();
		else
			horizontalBuffer->Detatch();

		VerticalPass.lock()->Render();

		if (output != nullptr && Detatch)
			output->Detatch();
	}

	std::shared_ptr<FrameBuffer> BlurOperation::GetHorizontalPass() const
	{
		return HorizontalBuffer.lock();
	}

	void BlurOperation::SetInput(const std::shared_ptr<Texture>& texture)
	{
		HorizontalPass.lock()->Input = texture;
	}
}
