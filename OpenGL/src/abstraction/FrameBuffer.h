#pragma once
class FrameBuffer
{
	private:
		unsigned int m_RendererID;
		int m_Width, m_Height;

	public:
		unsigned int ColorAttachmentID;
		unsigned int DepthAndStencilAttachmentID;

	public:
		FrameBuffer(int width, int height);
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& other) noexcept;
		FrameBuffer& operator=(FrameBuffer&& other) noexcept;

		void Bind() const;
		void Unbind() const;
		void AddColorAttachment();
		void AddDepthStencilAttachment();
		bool Validate();

};

