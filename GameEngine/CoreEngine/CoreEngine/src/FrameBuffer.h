#pragma once

#include <vector>
#include <glew.h>

#include "Dimensions.h"
#include "Object.h"

namespace GraphicsEngine
{
	class Texture;

	class FrameBuffer : public Engine::Object
	{
	public:
		~FrameBuffer();

		void Initialize();
		void Update(float) {}

		static Dimensions WindowSize;

		void DrawTo(int x = 0, int y = 0, int width = -1, int height = -1) const;
		void Bind() const;
		void Resize(int width, int height, bool resizeTextures = true);
		void Resize(Dimensions newSize, bool resizeTextures = true);
		std::shared_ptr<Texture> GetTexture(unsigned int attachmentSlot = 0) const;
		std::shared_ptr<Texture> GetDepthTexture() const;
		GLenum GetAttachmentSlot(unsigned int attachmentSlot = 0) const;
		GLuint GetID() const;
		Dimensions GetSize() const;
		void SetAttachmentCount(unsigned int attachments);
		void AttachTexture(unsigned int attachmentSlot, const std::shared_ptr<Texture>& texture, GLenum attachment, bool setParent = false, GLenum type = GL_TEXTURE_2D);
		void BindAttachments();
		void ClearTextures();
		int GetWidth() const;
		int GetHeight() const;
		void DisableDepthBuffer();

		static void SetViewport(int x, int y, int width, int height);
		static void Detatch();
		static std::shared_ptr<FrameBuffer> GetAttachedBuffer();
		static std::shared_ptr<FrameBuffer> Create(unsigned int width, unsigned int height, const std::shared_ptr<Texture>& colorBuffer0 = nullptr, bool setParent = false, bool depthBufferEnabled = true);

	private:
		typedef std::vector<GLenum> AttachmentVector;
		typedef std::vector<std::weak_ptr<Texture>> TextureVector;

		static std::shared_ptr<FrameBuffer>& CurrentBuffer();
		static Dimensions DrawSize;
		static Dimensions DrawPosition;

		int ObjectIDCache = -1;
		bool DepthBufferEnabled = true;
		std::weak_ptr<Texture> DepthTexture;
		Dimensions Size;
		GLuint BufferID = 0;
		GLuint FormerBufferID = 0; // for debug purposes, detecting double deletes
		AttachmentVector AttachmentSlots = AttachmentVector(1);
		TextureVector AttachmentTextures = TextureVector(1);

		Instantiable;

		Inherits_Class(Object);

		Reflected(FrameBuffer);
	};
}

namespace Engine
{
	//Class_Inherits(GraphicsEngine::FrameBuffer, Object);
}
