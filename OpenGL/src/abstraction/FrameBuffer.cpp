#include "FrameBuffer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace {
	GLenum ToGLAttachmentPoint(AttachmentTarget target, int colorIndex) 
	{
		switch (target)
		{
			case AttachmentTarget::Color:        return GL_COLOR_ATTACHMENT0 + colorIndex;
			case AttachmentTarget::Depth:        return GL_DEPTH_ATTACHMENT;
			case AttachmentTarget::Stencil:      return GL_STENCIL_ATTACHMENT;
			case AttachmentTarget::DepthStencil: return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		return 0;
	}

	GLenum ToGLInternalFormat(AttachmentTarget target)
	{
		switch (target)
		{
			case AttachmentTarget::Color:        return GL_RGBA8;
			case AttachmentTarget::Depth:        return GL_DEPTH_COMPONENT24;
			case AttachmentTarget::Stencil:      return GL_STENCIL_INDEX8;
			case AttachmentTarget::DepthStencil: return GL_DEPTH24_STENCIL8;
		}
		return 0;
	}
}

FrameBuffer::FrameBuffer(int width, int height, int samples) : m_Width(width), m_Height(height), m_MSAASamples(samples)
{
	GLCall(glGenFramebuffers(1, &m_RendererID));
}

FrameBuffer::~FrameBuffer()
{
	if (m_RendererID != 0)
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
	: m_RendererID(other.m_RendererID), m_Width(other.m_Width), m_Height(other.m_Height), m_Attachments(std::move(other.m_Attachments))
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
		m_Attachments = std::move(other.m_Attachments);

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

void FrameBuffer::Blit(const FrameBuffer& target) const
{

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.GetID());
	glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::AddAttachment(AttachmentTarget target, AttachmentStorage storage, int colorIndex)
{
	Bind();

	GLenum attachPoint = ToGLAttachmentPoint(target, colorIndex);
	Attachment attachment{ target, colorIndex, {} };

	if (storage == AttachmentStorage::Texture) 
	{
		std::unique_ptr<Texture> tex = std::make_unique<Texture>(Texture::CreateEmpty(m_Width, m_Height,ToGLInternalFormat(target), m_MSAASamples));
		GLCall(glNamedFramebufferTexture(m_RendererID, attachPoint, tex->GetId(), 0));
		attachment.storage = std::move(tex);
	}
	else 
	{
		std::unique_ptr<RenderBuffer> rbo = std::make_unique<RenderBuffer>(m_Width, m_Height, ToGLInternalFormat(target), m_MSAASamples);
		GLCall(glNamedFramebufferRenderbuffer(m_RendererID, attachPoint, GL_RENDERBUFFER, rbo->GetID()));
		attachment.storage = std::move(rbo);
	}

	if (target == AttachmentTarget::Color)
	{
		m_DrawBuffers.push_back(attachPoint);
		GLCall(glDrawBuffers(static_cast<GLsizei>(m_DrawBuffers.size()), m_DrawBuffers.data()));
	}

	m_Attachments.push_back(std::move(attachment));

	Unbind();
}

Texture* FrameBuffer::GetColorTexture(int colorIndex) const
{
	for (const Attachment& a : m_Attachments) 
	{
		if (a.target == AttachmentTarget::Color && a.colorIndex == colorIndex)
		{
			if (auto* tex = std::get_if<std::unique_ptr<Texture>>(&a.storage))
				return tex->get();
		}
	}
	std::cout << "Error: Was not able to locate Color attachment " << colorIndex << " For framebuffer.";
	return nullptr;
}
