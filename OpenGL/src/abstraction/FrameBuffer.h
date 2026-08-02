#pragma once
#include <memory>
#include "Texture.h"
#include "RenderBuffer.h"
#include <variant>
#include <vector>

enum class AttachmentTarget {
	Color, Depth, Stencil, DepthStencil
};
enum class AttachmentStorage {
	Texture, RenderBuffer
};

class FrameBuffer
{
	private:
		unsigned int m_RendererID;
		int m_Width, m_Height;

		struct Attachment
		{
			AttachmentTarget target;
			int colorIndex; // colorIndex only matters when target == Color
			std::variant<std::unique_ptr<Texture>, std::unique_ptr<RenderBuffer>> storage;
		};

		std::vector<Attachment> m_Attachments;
		std::vector<GLenum> m_DrawBuffers;

	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;
		bool Validate();

		void AddAttachment(AttachmentTarget target, AttachmentStorage storage, int colorIndex = 0); // colorIndex only matters when target == Color
		Texture* GetColorTexture(int colorIndex = 0) const;
};

