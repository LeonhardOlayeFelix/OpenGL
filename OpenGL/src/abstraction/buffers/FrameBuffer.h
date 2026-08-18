#pragma once
#include <memory>
#include "textures/Texture.h"
#include "textures/CubeMap.h"
#include "RenderBuffer.h"
#include <variant>
#include <vector>

enum class AttachmentTarget {
	Color, Depth, Stencil, DepthStencil
};
enum class AttachmentStorage {
	Texture, RenderBuffer, CubeMap
};

class FrameBuffer
{
	private:
		unsigned int m_RendererID{};
		int m_Width{}, m_Height{}, m_MSAASamples{};

		struct Attachment
		{
			AttachmentTarget target;
			int colorIndex; // colorIndex only matters when target == Color
			std::variant<Texture, RenderBuffer, CubeMap> storage;
		};

		std::vector<Attachment> m_Attachments{};
		std::vector<GLenum> m_DrawBuffers{};

	public:
		FrameBuffer() = default;
		FrameBuffer(int width, int height, int samples = 1);
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;

		void AddAttachment(AttachmentTarget target, AttachmentStorage storage, int colorIndex = 0); // colorIndex only matters when target == Color
		void Bind() const;
		void Unbind() const;
		bool Validate();
		void Blit(const FrameBuffer& fbo) const;
		void MarkAsNoColorBuffer();
		const Texture& GetColorTexture(int colorIndex = 0) const;
		const Texture& GetDepthTexture() const;
		const CubeMap& GetDepthCubeMap() const;

		inline unsigned int GetID() const { return m_RendererID; };
		inline unsigned int GetWidth() const { return m_Width; };
		inline unsigned int GetHeight() const { return m_Height; };
};

