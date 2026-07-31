#include "FrameBuffer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height) : m_Width(width), m_Height(height), ColorAttachmentID(0), DepthAndStencilAttachmentID(0)
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
	if (m_RendererID != 0)
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

FrameBuffer::FrameBuffer(FrameBuffer && other) noexcept
	: m_RendererID(other.m_RendererID), ColorAttachmentID(other.ColorAttachmentID), DepthAndStencilAttachmentID(other.DepthAndStencilAttachmentID)
{
	other.m_RendererID = 0;
	other.ColorAttachmentID = 0;
	other.DepthAndStencilAttachmentID = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer && other) noexcept
{
	if (this != &other) {
		glDeleteFramebuffers(1, &m_RendererID);
		m_RendererID = other.m_RendererID;
		ColorAttachmentID = other.ColorAttachmentID;
		DepthAndStencilAttachmentID = other.DepthAndStencilAttachmentID;

		other.m_RendererID = 0;
		other.ColorAttachmentID = 0;
		other.DepthAndStencilAttachmentID = 0;
	}

	return *this;
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::AddColorAttachment()
{
	Bind();
	glGenTextures(1, &ColorAttachmentID);
	glBindTexture(GL_TEXTURE_2D, ColorAttachmentID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachmentID, 0);
	Unbind();
}

void FrameBuffer::AddDepthStencilAttachment()
{
	Bind();
	glGenRenderbuffers(1, &DepthAndStencilAttachmentID);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthAndStencilAttachmentID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthAndStencilAttachmentID);
	Unbind();
}

bool FrameBuffer::Validate()
{
	Bind();
	bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
	if (result)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	Unbind();
	return result;
}
