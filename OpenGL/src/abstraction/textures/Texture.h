#pragma once
#include <string>
#include "ErrorHandling.h"
#include <GL/glew.h>

class Texture
{
private:
	unsigned int m_RendererID{};
	mutable unsigned int m_BoundSlot{};
	std::string m_FilePath{};
	unsigned char* m_LocalBuffer{};
	int m_Width{}, m_Height{}, m_BPP{};

public:
	Texture() = default;
	Texture(const std::string path, GLenum internalFormat = GL_RGBA8);
	static Texture CreateEmpty(int width, int height, GLenum internalFormat, int samples = 1);
	~Texture();

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Bind(unsigned int slot = 0) const ;
	void Unbind() const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
	inline unsigned int GetId() const { return m_RendererID; }
	inline unsigned int GetBoundSlot() const { return m_BoundSlot; }
};

