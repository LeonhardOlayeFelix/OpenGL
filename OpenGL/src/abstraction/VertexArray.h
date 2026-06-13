#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	unsigned int m_AttributeIndex = 0; //To know which attribute should be set next
	const IndexBuffer* m_IndexBuffer;
	size_t m_VBOSize;
	size_t m_Stride;
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void RecordVBOLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);
	void RecordIndexBuffer(const IndexBuffer& ibo);
	void SetAttribDivisor(unsigned int index, unsigned int divisor);
	inline unsigned int GetIndexBufferCount() const { return m_IndexBuffer->GetCount(); };
	inline size_t GetVertexBufferSize() const { return m_VBOSize; };
	inline size_t GetStride() const { return m_Stride; };
	void Bind() const;
	void Unbind() const;
};

