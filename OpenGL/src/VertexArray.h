#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"


class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void RecordVBOLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);
	void Bind() const;
	void Unbind() const;
};

