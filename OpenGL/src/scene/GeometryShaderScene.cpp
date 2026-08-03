#include "GeometryShaderScene.h"
#include "vendor/imgui/imgui.h"
#include <iostream>

scene::GeometryShaderScene::GeometryShaderScene()
{
	DoPreviousInit();

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

	renderer.DrawArray(*m_VAO, *m_Shader, GL_POINTS);

	m_Camera->UpdateCameraVectors();

}

void scene::GeometryShaderScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::GeometryShaderScene::DoPreviousInit()
{
	float points[] = {
	-0.25f,  0.25f,
	 0.25f,  0.25f,
	 0.25f, -0.25f,
	-0.25f, -0.25f 
	};

	VertexBufferLayout layout;
	layout.Push<float>(2);

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(points, sizeof(points));
	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/GeometryShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;
}
