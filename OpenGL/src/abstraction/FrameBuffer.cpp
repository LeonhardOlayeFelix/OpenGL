#include "FrameBuffer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


FrameBuffer::FrameBuffer(int width, int height) : m_Width(width), m_Height(height)
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
	if (m_RendererID != 0)
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
	: m_RendererID(other.m_RendererID),
	ColorAttachment(std::move(other.ColorAttachment)),
	DepthAndStencilAttachment(std::move(other.DepthAndStencilAttachment)),
	m_Width(other.m_Width), m_Height(other.m_Height)
{
	other.m_RendererID = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer && other) noexcept
{
	if (this != &other) {
		if (m_RendererID != 0)
			GLCall(glDeleteFramebuffers(1, &m_RendererID));
		m_RendererID = other.m_RendererID;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		ColorAttachment = std::move(other.ColorAttachment);
		DepthAndStencilAttachment = std::move(other.DepthAndStencilAttachment);

		other.m_RendererID = 0;
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
	ColorAttachment = std::make_unique<Texture>(Texture::CreateEmpty(m_Width, m_Height));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment->GetId(), 0));
	Unbind();
}

void FrameBuffer::AddDepthStencilAttachment()
{
	Bind();
	DepthAndStencilAttachment = std::make_unique<RenderBuffer>(m_Width, m_Height);
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilAttachment->GetID()));
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
