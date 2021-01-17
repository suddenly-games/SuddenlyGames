#include "FrameBuffer.h"

#include "Graphics.h"
#include "Textures.h"
#include "Texture.h"

namespace GraphicsEngine
{
	Dimensions FrameBuffer::WindowSize = Dimensions(0, 0);
	Dimensions FrameBuffer::DrawSize = Dimensions(0, 0);
	Dimensions FrameBuffer::DrawPosition = Dimensions(0, 0);

	void FrameBuffer::SetViewport(int x, int y, int width, int height)
	{
		if (
			width != DrawSize.Width || height != DrawSize.Height ||
			x != DrawPosition.Width || y != DrawPosition.Height
		)
			glViewport(x, y, width, height);

		DrawSize.Width = unsigned int(width);
		DrawSize.Height = unsigned int(height);
		DrawPosition.Width = unsigned int(x);
		DrawPosition.Height = unsigned int(y);
	}

	void FrameBuffer::Initialize()
	{
		ObjectIDCache = This.lock()->GetObjectID();

		std::shared_ptr<Texture> depthTexture = Engine::Create<Texture>();
		depthTexture->Name = "DepthTexture";
		depthTexture->SetParent(This.lock());
		depthTexture->Load(1, 1, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_UNSIGNED_INT_24_8, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL);

		DepthTexture = depthTexture;
	}

	std::shared_ptr<FrameBuffer>& FrameBuffer::CurrentBuffer()
	{
		static std::shared_ptr<FrameBuffer> current = nullptr;

		return current;
	}

	FrameBuffer::~FrameBuffer()
	{
		//if (FormerBufferID != 0 && BufferID == 0)
		//	throw "Attempt to double delete FrameBuffer";

		glBindFramebuffer(GL_FRAMEBUFFER, BufferID); CheckGLErrors();

		ClearTextures();

		if (CurrentBuffer() != nullptr)
			glBindFramebuffer(GL_FRAMEBUFFER, CurrentBuffer()->BufferID); CheckGLErrors();

		glDeleteFramebuffers(1, &BufferID); CheckGLErrors();

		FormerBufferID = BufferID;
		BufferID = 0;
	}

	void FrameBuffer::DrawTo(int x, int y, int width, int height) const
	{
		if (width == -1)
			width = int(Size.Width);

		if (height == -1)
			height = int(Size.Height);

		Bind();

		if (
			width != DrawSize.Width || height != DrawSize.Height ||
			x != DrawPosition.Width || y != DrawPosition.Height
		)
			glViewport(x, y, width, height);

		DrawSize.Width = unsigned int(width);
		DrawSize.Height = unsigned int(height);
		DrawPosition.Width = unsigned int(x);
		DrawPosition.Height = unsigned int(y);

		CheckGLErrors();
	}

	void FrameBuffer::Bind() const
	{
		if (&*CurrentBuffer() != this)
		{
			CurrentBuffer() = This.lock()->Cast<FrameBuffer>();

			glBindFramebuffer(GL_FRAMEBUFFER, BufferID); CheckGLErrors();
		}

		CheckGLErrors();
	}

	void FrameBuffer::Resize(int width, int height, bool resizeTextures)
	{
		Size.Width = width;
		Size.Height = height;

		DepthTexture.lock()->Resize(width, height);

		if (BufferID)
			glDeleteFramebuffers(1, &BufferID); CheckGLErrors();

		glGenFramebuffers(1, &BufferID); CheckGLErrors();

		glBindFramebuffer(GL_FRAMEBUFFER, BufferID); CheckGLErrors();

		if (DepthBufferEnabled)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthTexture.lock()->GetTextureID(), 0); CheckGLErrors();

		{
			for (unsigned int i = 0; i < AttachmentTextures.size(); ++i)
			{
				auto attached = AttachmentTextures[i].lock();

				if (attached != nullptr)
				{
					if (resizeTextures)
						attached->Resize(width, height);

					glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentSlots[i], GL_TEXTURE_2D, attached->GetTextureID(), 0); CheckGLErrors();
				}
			}
		}

		BindAttachments(); CheckGLErrors();

		Detatch(); CheckGLErrors();
	}

	void FrameBuffer::Resize(Dimensions newSize, bool resizeTextures)
	{
		Resize(newSize.Width, newSize.Height, resizeTextures);
	}

	std::shared_ptr<Texture> FrameBuffer::GetTexture(unsigned int attachmentSlot) const
	{
		return AttachmentTextures[attachmentSlot].lock();
	}

	std::shared_ptr<Texture> FrameBuffer::GetDepthTexture() const
	{
		return DepthTexture.lock();
	}

	GLenum FrameBuffer::GetAttachmentSlot(unsigned int attachmentSlot) const
	{
		return AttachmentSlots[attachmentSlot];
	}

	Dimensions FrameBuffer::GetSize() const
	{
		return Size;
	}

	GLuint FrameBuffer::GetID() const
	{
		return BufferID;
	}

	void FrameBuffer::SetAttachmentCount(unsigned int attachments)
	{
		AttachmentSlots.resize(attachments);
		AttachmentTextures.resize(attachments);
	}

	void FrameBuffer::AttachTexture(unsigned int attachmentSlot, const std::shared_ptr<Texture>& texture, GLenum attachment, bool setParent, GLenum type)
	{
		if (AttachmentTextures.size() <= attachmentSlot)
		{
			int size = AttachmentTextures.size();
			AttachmentTextures.resize(attachmentSlot + 1);
			AttachmentSlots.resize(attachmentSlot + 1);
		}

		AttachmentTextures[attachmentSlot] = texture;
		AttachmentSlots[attachmentSlot] = attachment;

		if (texture != nullptr)
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, texture->GetTextureID(), 0);
		else
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, type, 0, 0);

		CheckGLErrors();

		if (setParent)
		{
			char buffer[20] = { 0 };

			std::stringstream name(buffer);

			name << "ColorAttachment" << attachmentSlot;

			texture->Name = name.str();
			texture->SetParent(This.lock());
		}
	}

	void FrameBuffer::BindAttachments()
	{
		glDrawBuffers(AttachmentSlots.size(), AttachmentSlots.data()); CheckGLErrors();
	}

	void FrameBuffer::ClearTextures()
	{
		for (unsigned int i = 0; i < AttachmentTextures.size(); ++i)
		{
			if (!AttachmentTextures[i].expired())
			{
				glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentSlots[i], GL_TEXTURE_2D, 0, 0); CheckGLErrors();

				AttachmentTextures[i].reset();
			}
		}
	}

	int FrameBuffer::GetWidth() const
	{
		return Size.Width;
	}

	int FrameBuffer::GetHeight() const
	{
		return Size.Height;
	}

	void FrameBuffer::DisableDepthBuffer()
	{
		DepthBufferEnabled = false;
	}

	void FrameBuffer::Detatch()
	{
		if (CurrentBuffer() != nullptr)
		{
			CurrentBuffer().reset();

			glBindFramebuffer(GL_FRAMEBUFFER, 0); CheckGLErrors();
		}

		if (
			DrawPosition.Width != 0 || DrawPosition.Height != 0 ||
			DrawSize.Width != WindowSize.Width || DrawSize.Height != WindowSize.Height
		)
			glViewport(0, 0, WindowSize.Width, WindowSize.Height); CheckGLErrors();

		DrawSize.Width = WindowSize.Width;
		DrawSize.Height = WindowSize.Height;
		DrawPosition.Width = 0;
		DrawPosition.Height = 0;
	}

	std::shared_ptr<FrameBuffer> FrameBuffer::GetAttachedBuffer()
	{
		return CurrentBuffer();
	}

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(unsigned int width, unsigned int height, const std::shared_ptr<Texture>& colorBuffer0, bool setParent, bool depthBufferEnabled)
	{
		std::shared_ptr<FrameBuffer> buffer = Engine::Create<FrameBuffer>();

		if (colorBuffer0 != nullptr)
		{
			buffer->SetAttachmentCount(0);

			colorBuffer0->SetParent(buffer);
		}

		if (!depthBufferEnabled)
			buffer->DisableDepthBuffer();

		buffer->Resize(width, height);
		buffer->Bind();

		if (colorBuffer0 != nullptr)
		{
			buffer->SetAttachmentCount(1);
			buffer->AttachTexture(0, colorBuffer0, GL_COLOR_ATTACHMENT0, setParent);
			buffer->BindAttachments();
		}

		FrameBuffer::Detatch();

		return buffer;
	}
}
