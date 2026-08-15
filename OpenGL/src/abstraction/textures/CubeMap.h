#pragma once
#include <string>
#include "ErrorHandling.h"
#include <GL/glew.h>
#include <array>

enum CubemapFace { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK };

class CubeMap
{
private:
	unsigned int m_RendererID{};
	std::array<std::string, 6> m_FilePaths{};
	unsigned char* m_LocalBuffer{};
	int m_Width{}, m_Height{}, m_BPP{};

public:
	CubeMap() = default;
	CubeMap(const std::string& path);
	CubeMap(const std::array<std::string, 6>& paths);
	~CubeMap();

	CubeMap(CubeMap&& other) noexcept;
	CubeMap& operator=(CubeMap&& other) noexcept;
	CubeMap(const CubeMap&) = delete;
	CubeMap& operator=(const CubeMap&) = delete;

	void Bind(unsigned int slot = 0) const ;
	void Unbind(unsigned int slot) const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
	inline int GetId() { return m_RendererID; }
};

