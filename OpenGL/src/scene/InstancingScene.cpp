#include "InstancingScene.h"
#include <GLFW/glfw3.h>
#include "vendor/imgui/imgui.h"

scene::InstancingScene::InstancingScene()
{
	DoPreviousInit();

	float vertices[] = {
		-0.05f, 0.05f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.05f, 0.05f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.05f,-0.05f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.05f,-0.05f, 0.0f, 0.2f, 0.5f, 0.8f,
	};

	const unsigned int indicies[] = {
		0, 1, 2,
		0, 2, 3
	};

	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2) {
		for (int x = -10; x < 10; x += 2) {
			glm::vec3 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translation.z = 0;
			translations[index++] = translation;
		}
	}

	m_VAO = VertexArray();
	m_VAO.Bind();

	m_VBO = VertexBuffer(vertices, sizeof(vertices));
	
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);

	m_VAO.RecordVBOLayout(m_VBO, vbl);

	m_InstanceVBO = VertexBuffer(&translations[0], sizeof(translations));

	VertexBufferLayout vbl2;
	vbl2.Push<float>(3);

	m_VAO.RecordVBOLayout(m_InstanceVBO, vbl2);
	m_VAO.SetAttribDivisor(2, 1);

	m_IBO = IndexBuffer(indicies, sizeof(indicies) / sizeof(unsigned int));

	m_VAO.RecordIndexBuffer(m_IBO);

	m_Shader = ShaderProgram("res/shaders/InstancingSceneShader.shader");
	m_Shader.Bind();
}

scene::InstancingScene::~InstancingScene()
{
}

void scene::InstancingScene::OnUpdate(double deltaTime, GLFWwindow* window)
{
	m_Window = window;

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::UP, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Camera.ProcessKeyboard(CameraMovement::DOWN, deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_J))
		m_Camera.ProcessMouseMovement(-500 * deltaTime, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_L))
		m_Camera.ProcessMouseMovement(500 * deltaTime, 0);
	if (glfwGetKey(m_Window, GLFW_KEY_I))
		m_Camera.ProcessMouseMovement(0, 500 * deltaTime);
	if (glfwGetKey(m_Window, GLFW_KEY_K))
		m_Camera.ProcessMouseMovement(0, -500 * deltaTime);
}

void scene::InstancingScene::OnRender()
{
	Renderer renderer;

	renderer.DrawElementsInstanced(m_VAO, m_Shader, NR_OBJECTS);

	m_Camera.UpdateCameraVectors();
}

void scene::InstancingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();


	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::InstancingScene::DoPreviousInit()
{
	m_Camera = Camera(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);
}
