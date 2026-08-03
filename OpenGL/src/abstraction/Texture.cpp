#include "Texture.h"
#include "ErrorHandling.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string path) : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));

	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
	GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

	GLCall(glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height));
	GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glGenerateTextureMipmap(m_RendererID));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture Texture::CreateEmpty(int width, int height, GLenum internalFormat) 
{
	Texture tex;
	tex.m_Width = width;
	tex.m_Height = height;
	tex.m_LocalBuffer = nullptr;

	GLenum format, type;
	switch (internalFormat)
	{
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32F:
			format = GL_DEPTH_COMPONENT;
			type = GL_FLOAT;
			break;
		case GL_DEPTH24_STENCIL8:
			format = GL_DEPTH_STENCIL;
			type = GL_UNSIGNED_INT_24_8;
			break;
		default:
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
	}

	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &tex.m_RendererID));
	GLCall(glTextureStorage2D(tex.m_RendererID, 1, internalFormat, width, height));
	GLCall(glTextureParameteri(tex.m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTextureParameteri(tex.m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	return tex;
}

Texture::~Texture()
{
	if (m_RendererID != 0)
		GLCall(glDeleteTextures(1, &m_RendererID));
}

Texture::Texture(Texture&& other) noexcept
	: m_RendererID(other.m_RendererID), m_FilePath(other.m_FilePath),
	m_Width(other.m_Width), m_Height(other.m_Height), m_BPP(other.m_BPP)
{
	other.m_RendererID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other) {
		if (m_RendererID != 0)
			glDeleteTextures(1, &m_RendererID);

		m_RendererID = other.m_RendererID;
		m_FilePath = other.m_FilePath;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_BPP = other.m_BPP;

		other.m_RendererID = 0;
	}
	return *this;
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glBindTextureUnit(slot, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
