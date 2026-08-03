#include "GeometryShaderScene.h"
#include "vendor/imgui/imgui.h"
#include <iostream>

scene::GeometryShaderScene::GeometryShaderScene()
{
	DoPreviousInit();

	m_Model = std::make_unique<Model>("res/models/backpack/backpack.obj");

	m_UBO = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4));
	m_UBO->BindToPoint(0);

	glm::mat4 projMatrix = m_Camera->GetPerspectiveMatrix();
	m_UBO->SetData(glm::value_ptr(projMatrix), sizeof(glm::mat4), 0);
}

scene::GeometryShaderScene::~GeometryShaderScene()
{
}

void scene::GeometryShaderScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::GeometryShaderScene::OnRender()
{
	Renderer renderer;

	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	m_UBO->SetData(glm::value_ptr(viewMatrix), sizeof(glm::mat4), sizeof(glm::mat4));

	m_ExplosionShader->SetUniformMat4f("u_Model", modelMatrix);
	m_ExplosionShader->SetUniform1f("u_Magnitude", m_Magnitude);
	m_Model->Draw(*m_ExplosionShader);
	
	m_NormalShader->SetUniformMat4f("u_Model", modelMatrix);
	m_NormalShader->SetUniform1f("u_Magnitude", m_Magnitude);
	m_Model->Draw(*m_NormalShader);

	m_Camera->UpdateCameraVectors();

}

void scene::GeometryShaderScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SliderFloat("Magnitude", &m_Magnitude, 0, 2);

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::GeometryShaderScene::DoPreviousInit()
{

	m_ExplosionShader = std::make_unique<ShaderProgram>("res/shaders/GeometryExplosionShader.shader");
	m_ExplosionShader->SetUniformBlockBinding("Matrices", 0);

	m_NormalShader = std::make_unique<ShaderProgram>("res/shaders/GeometryNormalShader.shader");
	m_NormalShader->SetUniformBlockBinding("Matrices", 0);

	m_Camera = std::make_unique<Camera>(glm::vec3(0, 0, 10), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	m_Camera->Fov = 80;
}
