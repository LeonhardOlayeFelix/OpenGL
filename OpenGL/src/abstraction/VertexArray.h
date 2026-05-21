#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	const IndexBuffer* m_IndexBuffer;
	int m_VBOCount;
public:
	VertexArray();
	~VertexArray();

	void RecordVBOLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);
	void RecordIndexBuffer(const IndexBuffer& ibo);
	inline unsigned int GetIndexBufferCount() const { return m_IndexBuffer->GetCount(); };
	inline int GetVertexBufferCount() const { return m_VBOCount; };
	void Bind() const;
	void Unbind() const;
};

