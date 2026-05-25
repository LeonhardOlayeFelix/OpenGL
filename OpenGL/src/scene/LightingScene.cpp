#include "LightingScene.h"
#include "vendor/imgui/imgui.h"

scene::LightingScene::LightingScene()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();

	m_LightVAO = std::make_unique<VertexArray>();
	m_LightVAO->Bind();
	m_LightVAO->RecordVBOLayout(*m_VBO, layout);
	m_LightVAO->Unbind();

	m_ObjectShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneShader.shader");

	m_LampShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneSourceShader.shader");

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
		m_Camera->ProcessMouseMovement(-500 * deltaTime, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_L))
		m_Camera->ProcessMouseMovement(500 * deltaTime, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_I))
		m_Camera->ProcessMouseMovement(0, 500 * deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_K))
		m_Camera->ProcessMouseMovement(0, -500 * deltaTime);
}

void scene::LightingScene::OnRender()
{

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();

	Renderer renderer;

	m_VAO->Bind();
	m_ObjectShader->Bind();
	m_ObjectShader->SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_ObjectShader->SetUniformMat4f("u_Model", glm::mat4(1.0f));
	m_ObjectShader->SetUniform3f("u_Albedo", m_ObjectAlbedo);
	m_ObjectShader->SetUniform3f("m_LightIntensity", m_LightIntensity);
	m_ObjectShader->SetUniform3f("u_LightPosition", m_LightPosition);
	m_ObjectShader->SetUniform1i("u_UseAmbientLighting", m_UseAmbientLighting);
	m_ObjectShader->SetUniform1i("u_UseDiffuseLighting", m_UseDiffuseLighting);
	renderer.DrawArray(*m_VAO, *m_ObjectShader);


	m_LightVAO->Bind();
	m_LampShader->Bind();
	m_LampShader->SetUniformMat4f("u_View", viewMatrix);
	m_LampShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_LampShader->SetUniformMat4f("u_Model", glm::translate(glm::mat4(1.0f), m_LightPosition));
	m_LampShader->SetUniform3f("u_LightColor", m_LightIntensity);
	renderer.DrawArray(*m_LightVAO, *m_LampShader);

	m_Camera->UpdateCameraVectors();
}

void scene::LightingScene::OnImGuiRender()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Object"))
	{
		ImGui::ColorPicker3("Albedo", glm::value_ptr(m_ObjectAlbedo));
	}

	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::SliderFloat3("Position", glm::value_ptr(m_LightPosition), -5.0f, 5.0f);
		ImGui::SliderFloat3("Intensity", glm::value_ptr(m_LightIntensity), 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Checkbox("Ambient Lighting", &m_UseAmbientLighting);
		ImGui::Checkbox("Diffuse Lighting", &m_UseDiffuseLighting);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

}


