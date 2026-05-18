#include "TriangleScene.h"
#include <GLFW/glfw3.h>

scene::TriangleScene::TriangleScene()
{
	float positions[] = {
		-0.25f, -0.25f, 0.0f, 1.0f, 0.00f, 0.00f, 1.0f,
		 0.25f, -0.25f, 0.0f, 0.00f, 1.0f, 0.00f, 1.0f,
		 0.0f,  0.25f, 0.0f, 0.00f, 0.00f, 1.0f, 1.0f
	};

	unsigned int indicies[] = {
		0, 1, 2
	};

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);

	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_IBO = std::make_unique<IndexBuffer>(indicies, 3);

	m_VAO->RecordIndexBuffer(*m_IBO);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/TriangleSceneShader.shader");
	m_Shader->Bind();
}

scene::TriangleScene::~TriangleScene()
{
}

void scene::TriangleScene::OnUpdate(float deltaTime)
{
}

void scene::TriangleScene::OnRender()
{
	Renderer renderer;

	m_Shader->SetUniform1f("u_Opacity", (sin(4 * glfwGetTime() / 2) / 2.0f) + 0.5f);
	m_Shader->SetUniform1f("u_XOffset", sin(glfwGetTime()) / 2);
	m_Shader->SetUniform1f("u_YOffset", cos(glfwGetTime()) / 2);

	renderer.Draw(*m_VAO, *m_Shader);

}

void scene::TriangleScene::OnImGuiRender()
{
}
