#include "Renderer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>

Renderer::Renderer()
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));
}

void Renderer::Draw(const VertexArray& vao, const ShaderProgram& shader) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, vao.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.12f, 0.12f, 0.12f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
