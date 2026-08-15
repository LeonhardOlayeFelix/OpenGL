#pragma once
#include <array>
class VertexBuffer {
private:
	unsigned int m_RendererID{};
	unsigned int m_count{};
public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, size_t size);
	template<size_t N>
	VertexBuffer(const std::array<float, N>& data) : VertexBuffer(data.data(), N * sizeof(float)) {};
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void Bind() const;
	void Unbind() const;
	void UpdateData(const void* data, size_t size);
	inline unsigned int GetCount() const { return m_count; };
	inline unsigned int GetID() const { return m_RendererID; };
};