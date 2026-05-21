#pragma once
class VertexBuffer {
private:
	unsigned int m_RendererID;
	int m_count;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateData(const void* data, unsigned int size);
	inline int GetCount() const { return m_count; };
};