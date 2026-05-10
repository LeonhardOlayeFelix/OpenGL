#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
	const IndexBuffer* m_IndexBuffer;
public:
	VertexArray();
	~VertexArray();

	void RecordVBOLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);
	void RecordIndexBuffer(const IndexBuffer& ibo);
	void Bind() const;
	void Unbind() const;
};

