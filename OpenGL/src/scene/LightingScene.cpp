#include "LightingScene.h"
#include "vendor/imgui/imgui.h"

scene::LightingScene::LightingScene()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();

	m_LightVAO = std::make_unique<VertexArray>();
	m_LightVAO->Bind();
	m_LightVAO->RecordVBOLayout(*m_VBO, layout);
	m_LightVAO->Unbind();

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/LightSceneShader.shader");

	m_LightShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneSourceShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0f);
}

scene::LightingScene::~LightingScene()
{
}

void scene::LightingScene::OnUpdate(double deltaTime, GLFWwindow* window)
{
	m_Window = window;

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_J))
		m_Camera->ProcessMouseMovement(-10, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_L))
		m_Camera->ProcessMouseMovement(10, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_I))
		m_Camera->ProcessMouseMovement(0, 10);
	if (glfwGetKey(m_Window, GLFW_KEY_K))
		m_Camera->ProcessMouseMovement(0, -10);
}

void scene::LightingScene::OnRender()
{
	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();

	Renderer renderer;

	m_VAO->Bind();
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_View", viewMatrix);
	m_Shader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_Shader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
	m_Shader->SetUniform3f("u_Albedo", glm::vec3(1, 0, 0));
	m_Shader->SetUniform3f("u_LightColor", glm::vec3(1, 1, 1));
	renderer.DrawArray(*m_VAO, *m_Shader);


	m_LightVAO->Bind();
	m_LightShader->Bind();
	m_LightShader->SetUniformMat4f("u_View", viewMatrix);
	m_LightShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_LightShader->SetUniformMat4f("u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(2.0)));
	m_LightShader->SetUniform3f("u_LightColor", glm::vec3(1, 1, 1));
	renderer.DrawArray(*m_LightVAO, *m_LightShader);

	m_Camera->UpdateCameraVectors();
}

void scene::LightingScene::OnImGuiRender()
{
	ImGui::BeginDisabled(true);
	ImGui::SliderFloat("Fov", &m_Camera->Fov, 0.0f, 90.0f);
	ImGui::SliderFloat("Yaw", &m_Camera->Yaw, -180.0f, 180.0f);
	ImGui::SliderFloat("Pitch", &m_Camera->Pitch, -180.0f, 180.0f);
	ImGui::SliderFloat3("Camera Position", glm::value_ptr(m_Camera->Position), -10.0f, 10.0f);
	ImGui::SliderFloat3("Camera Up", glm::value_ptr(m_Camera->Up), -1.0f, 1.0f);
	ImGui::SliderFloat3("Camera Front", glm::value_ptr(m_Camera->Front), -1.0f, 1.0f);
	ImGui::EndDisabled();
}


