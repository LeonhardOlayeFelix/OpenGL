#include "VertexArray.h"
#include "ErrorHandling.h"

VertexArray::VertexArray() : m_VBOSize(0), m_Stride(0)
{
	GLCall(glCreateVertexArrays(1, &m_RendererID));
}
VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_RendererID(other.m_RendererID), m_IndexBuffer(other.m_IndexBuffer),
	m_VBOSize(other.m_VBOSize), m_Stride(other.m_Stride), m_AttributeIndex(other.m_AttributeIndex)
{
	other.m_RendererID = 0;
	other.m_IndexBuffer = nullptr;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other) {
		glDeleteVertexArrays(1, &m_RendererID);
		m_RendererID = other.m_RendererID;
		m_IndexBuffer = other.m_IndexBuffer;
		m_VBOSize = other.m_VBOSize;
		m_Stride = other.m_Stride;
		m_AttributeIndex = other.m_AttributeIndex;
		other.m_RendererID = 0;
		other.m_IndexBuffer = nullptr;
	}
	return *this;
}
VertexArray::~VertexArray()
{
	if (m_RendererID != 0)
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
	m_VBOSize += vbo.GetCount();
	m_Stride += vbl.GetStride();

	const std::vector<VertexAttribute>& elements = vbl.GetAttributes();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {

		const VertexAttribute& attribute = elements[i];

		GLCall(glEnableVertexAttribArray(m_AttributeIndex));

		GLCall(glVertexAttribPointer(m_AttributeIndex, attribute.count, attribute.type, attribute.normalised, vbl.GetStride(), (const void*) offset));

		offset += attribute.count * VertexAttribute::GetSizeOfType(attribute.type);

		m_AttributeIndex++;
	}

}

void VertexArray::RecordIndexBuffer(const IndexBuffer& ibo)
{
	Bind();
	ibo.Bind();
	m_IndexBuffer = &ibo;
}

void VertexArray::SetAttribDivisor(unsigned int index, unsigned int divisor)
{
	Bind();
	GLCall(glVertexAttribDivisor(index, divisor));
}






