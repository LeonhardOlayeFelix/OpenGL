#pragma once
#include <string>
#include "ErrorHandling.h"
#include <GL/glew.h>

class Texture
{
private:
	Texture() = default;
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string path);
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
	inline int GetId() { return m_RendererID; }
};

