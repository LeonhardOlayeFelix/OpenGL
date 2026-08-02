#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(int width, int height, GLenum internalFormat) : m_RendererID(0), m_Width(width), m_Height(height)
{
    GLCall(glGenRenderbuffers(1, &m_RendererID));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

RenderBuffer::~RenderBuffer()
{
    if (m_RendererID != 0)
        GLCall(glDeleteRenderbuffers(1, &m_RendererID));
}

RenderBuffer::RenderBuffer(RenderBuffer&& other) noexcept
    : m_RendererID(other.m_RendererID), m_Width(other.m_Width), m_Height(other.m_Height)
{
    other.m_RendererID = 0;
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other) noexcept
{
    if (this != &other) {
        if (m_RendererID != 0)
            GLCall(glDeleteRenderbuffers(1, &m_RendererID));
        m_RendererID = other.m_RendererID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        other.m_RendererID = 0;
    }
    return *this;
}

void RenderBuffer::Bind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}

void RenderBuffer::Unbind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
