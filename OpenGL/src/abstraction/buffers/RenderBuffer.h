#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorHandling.h"

class RenderBuffer
{
public:
    RenderBuffer() = default;
    RenderBuffer(int width, int height, GLenum internalFormat = GL_DEPTH24_STENCIL8, int samples = 1);
    ~RenderBuffer();

    RenderBuffer(const RenderBuffer&) = delete;
    RenderBuffer& operator=(const RenderBuffer&) = delete;

    RenderBuffer(RenderBuffer&& other) noexcept;
    RenderBuffer& operator=(RenderBuffer&& other) noexcept;

    void Bind() const;
    void Unbind() const;

    unsigned int GetID() const { return m_RendererID; }

private:
    unsigned int m_RendererID{};
    int m_Width{}, m_Height{};
};

