#include "Renderer.h"
#include "ErrorHandling.h"
#include <GL/glew.h>

Renderer::Renderer()
{
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));
	GLCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::DrawElements(const VertexArray& vao, const ShaderProgram& shader) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, vao.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawElementsInstanced(const VertexArray& vao, const ShaderProgram& shader, int nInstances) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, vao.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr, nInstances));
}

void Renderer::DrawArray(const VertexArray& vao, const ShaderProgram& shader) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, vao.GetVertexBufferSize() / vao.GetStride()));
}

void Renderer::DrawArrayInstanced(const VertexArray& vao, const ShaderProgram& shader, int nInstances) const
{
	shader.Bind();
	vao.Bind();
	GLCall(glDrawElementsInstanced(GL_TRIANGLES, vao.GetIndexBufferCount(), GL_UNSIGNED_INT, nullptr, nInstances));
}


void Renderer::Clear() const
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}
