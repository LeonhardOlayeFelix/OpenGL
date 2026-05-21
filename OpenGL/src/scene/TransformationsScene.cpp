#include "TransformationsScene.h"
#include "vendor/imgui/imgui.h"
#include <GLFW/glfw3.h>

scene::TransformationsScene::TransformationsScene()
{

	m_TranslateX = 0.0f;
	m_TranslateY = 0.0f;
	m_TranslateZ = 0.0f;
	m_Angle = 0.0f;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_ScaleZ = 1.0f;

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f
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

	m_IBO = std::make_unique<IndexBuffer>(indicies, sizeof(indicies) / sizeof(unsigned int));

	m_VAO->RecordIndexBuffer(*m_IBO);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/TransformationsSceneShader.shader");
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Shader->SetUniform1i("u_Texture2", 1);

	m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
	m_Texture->Bind(0);

	m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
	m_Texture2->Bind(1);

	// Below code transforms model coordinates into world coordinates. 
	// By expressing the model orientation as a matrix M
	// M gets us from points expressed in M's point of view to the worlds point of view.

	m_ModelTransform = glm::mat4(1.0f);
	m_ModelTransform = glm::translate(m_ModelTransform, glm::vec3(m_TranslateX, m_TranslateY, m_TranslateZ));
	m_ModelTransform = glm::rotate(m_ModelTransform, glm::degrees(m_Angle), glm::vec3(0.0, 0.0, 1.0));
	m_ModelTransform = glm::scale(m_ModelTransform, glm::vec3(m_ScaleX, m_ScaleY, m_ScaleZ));
	
	m_Shader->SetUniformMat4f("u_Model", m_ModelTransform);
	
}

scene::TransformationsScene::~TransformationsScene()
{
}

void scene::TransformationsScene::OnUpdate(float deltaTime, GLFWwindow* window)
{
}

void scene::TransformationsScene::OnRender()
{
	Renderer renderer;
	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Texture2->Bind(1);
	renderer.DrawElements(*m_VAO, *m_Shader);

	 

}

void scene::TransformationsScene::OnImGuiRender()
{

	ImGui::SliderFloat("Translation X", &m_TranslateX, -0.5f, 0.5f);
	ImGui::SliderFloat("Translation Y", &m_TranslateY, -0.5f, 0.5f);
	ImGui::SliderFloat("Translation Z", &m_TranslateZ, -0.5f, 0.5f);
	ImGui::SliderAngle("Angle", &m_Angle, 0.0f, 360.0f);
	ImGui::SliderFloat("Scale X", &m_ScaleX, 0.0f, 2.0f);
	ImGui::SliderFloat("Scale Y", &m_ScaleY, 0.0f, 2.0f);
	ImGui::SliderFloat("Scale Z", &m_ScaleZ, 0.0f, 2.0f);

	m_ModelTransform = glm::mat4(1.0f);
	m_ModelTransform = glm::translate(m_ModelTransform, glm::vec3(m_TranslateX, m_TranslateY, m_TranslateZ));
	m_ModelTransform = glm::rotate(m_ModelTransform, glm::degrees(m_Angle / 180.0f * 3.14f), glm::vec3(0.0, 0.0, 1.0));
	m_ModelTransform = glm::scale(m_ModelTransform, glm::vec3(m_ScaleX, m_ScaleY, m_ScaleZ));

	m_Shader->SetUniformMat4f("u_Model", m_ModelTransform);


}
