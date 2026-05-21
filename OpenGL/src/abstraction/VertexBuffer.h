#pragma once
class VertexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_count;
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateData(const void* data, size_t size);
	inline unsigned int GetCount() const { return m_count; };
};