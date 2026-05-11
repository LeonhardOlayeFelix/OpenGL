#include "Renderer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>

void Renderer::Draw(const VertexArray& vao, const ShaderProgram& shader) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, vao.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
