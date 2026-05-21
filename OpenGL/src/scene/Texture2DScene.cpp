#include "Texture2DScene.h"
#include <GLFW/glfw3.h>
#include "vendor/imgui/imgui.h"

scene::Texture2DScene::Texture2DScene()
{
	m_float = 2.0f;
	m_float2 = 0.2f;

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   m_float, m_float,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   m_float, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, m_float
	};

	unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(4);
	layout.Push<float>(2);

	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_IBO = std::make_unique<IndexBuffer>(indicies, sizeof(indicies)/sizeof(unsigned int));

	m_VAO->RecordIndexBuffer(*m_IBO);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/Texture2DSceneShader.shader");
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Shader->SetUniform1i("u_Texture2", 1);
	m_Shader->SetUniform1f("u_Float", m_float2);

	m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
	m_Texture->Bind(0);

	m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
	m_Texture2->Bind(1);

}

scene::Texture2DScene::~Texture2DScene()
{
}

void scene::Texture2DScene::OnUpdate(float deltaTime)
{
}

void scene::Texture2DScene::OnRender()
{
	Renderer renderer;

	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Texture2->Bind(1);
	renderer.DrawElements(*m_VAO, *m_Shader);
}

void scene::Texture2DScene::OnImGuiRender()
{
	if (ImGui::SliderFloat("Float", &m_float, 1.0f, 10.0f))
	{
		float vertices[] = {
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   m_float, m_float,
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   m_float, 0.0f,
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f,    0.0f,
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f,    m_float
		};

		m_VBO->Bind();
		m_VBO->UpdateData(vertices, sizeof(vertices));
	}
	if (ImGui::SliderFloat("Float 2", &m_float2, 0.0f, 1.0f)) {
		m_Shader->SetUniform1f("u_Float", m_float2);
	}
}
