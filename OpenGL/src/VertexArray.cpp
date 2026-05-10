#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBuffer.h"

VertexArray::VertexArray() : m_IndexBuffer(nullptr)
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}
void VertexArray::Bind() const {
	glBindVertexArray(m_RendererID);
}
void VertexArray::Unbind() const {
	glBindVertexArray(0);
}
void VertexArray::RecordVBOLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl)
{
	Bind();
	vbo.Bind(); 

	const std::vector<VertexAttribute>& elements = vbl.GetElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {

		const VertexAttribute& attribute = elements[i];

		GLCall(glEnableVertexAttribArray(i));

		GLCall(glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalised, vbl.GetStride(), (const void*) offset));

		offset += attribute.count * VertexAttribute::GetSizeOfType(attribute.type);
	}
}

void VertexArray::RecordIndexBuffer(const IndexBuffer& ibo)
{
	ibo.Bind();
	m_IndexBuffer = &ibo;
}




