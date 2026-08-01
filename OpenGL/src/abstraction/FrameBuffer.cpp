#include "FrameBuffer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


FrameBuffer::FrameBuffer(int width, int height) : m_Width(width), m_Height(height), DepthAndStencilAttachmentID(0)
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
	if (m_RendererID != 0)
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
	if (DepthAndStencilAttachmentID != 0)
		GLCall(glDeleteRenderbuffers(1, &DepthAndStencilAttachmentID));
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
	: m_RendererID(other.m_RendererID),
	DepthAndStencilAttachmentID(other.DepthAndStencilAttachmentID),
	ColorAttachmentTexture(std::move(other.ColorAttachmentTexture)),
	m_Width(other.m_Width), m_Height(other.m_Height)
{
	other.m_RendererID = 0;
	other.DepthAndStencilAttachmentID = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer && other) noexcept
{
	if (this != &other) {
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
		m_RendererID = other.m_RendererID;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		DepthAndStencilAttachmentID = other.DepthAndStencilAttachmentID;
		ColorAttachmentTexture = std::move(other.ColorAttachmentTexture);


		other.m_RendererID = 0;
		other.DepthAndStencilAttachmentID = 0;
	}

	return *this;
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::AddColorAttachment()
{
	Bind();
	ColorAttachmentTexture = std::make_unique<Texture>(Texture::CreateEmpty(m_Width, m_Height));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachmentTexture->GetId(), 0));
	Unbind();
}

void FrameBuffer::AddDepthStencilAttachment()
{
	Bind();
	GLCall(glGenRenderbuffers(1, &DepthAndStencilAttachmentID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, DepthAndStencilAttachmentID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilAttachmentID));
	Unbind();
}

bool FrameBuffer::Validate()
{
	Bind();
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	bool isComplete = (status == GL_FRAMEBUFFER_COMPLETE);
	if (!isComplete)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! Status: " << status << std::endl;
	Unbind();
	return isComplete;
}
