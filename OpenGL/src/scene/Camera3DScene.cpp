#include "Camera3DScene.h"
#include "vendor/imgui/imgui.h"

scene::Camera3DScene::Camera3DScene()
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
	m_CubePositions = {
	glm::vec3(0.0f,  4.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/Transformations3DSceneShader.shader");
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Shader->SetUniform1i("u_Texture2", 1);

	m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
	m_Texture->Bind(0);

	m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
	m_Texture2->Bind(1);

	m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0f);
}

scene::Camera3DScene::~Camera3DScene()
{
	glfwSetCursorPosCallback(m_Window, nullptr);
	glfwSetMouseButtonCallback(m_Window, nullptr);
	glfwSetScrollCallback(m_Window, nullptr);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void scene::Camera3DScene::OnUpdate(double deltaTime, GLFWwindow* window)
{
	m_Window = window;
	if (!m_CallbacksSet) SetCallbacks();

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
}

void scene::Camera3DScene::OnRender()
{
	Renderer renderer;
	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Texture2->Bind(1);
	 
	m_Shader->SetUniformMat4f("u_View", m_Camera->GetViewMatrix());

	m_Shader->SetUniformMat4f("u_Proj", m_Camera->GetPerspectiveMatrix());

	for (unsigned int i = 0; i < 10; i++) {
		glm::mat4 modelTransform = glm::mat4(1.0f);
		modelTransform = glm::translate(modelTransform, m_CubePositions[i]);
		modelTransform = glm::rotate(modelTransform, glm::degrees((float)glfwGetTime() / 180.0f * 3.14f), glm::vec3(1.0, 1.0, -1.0));
		m_Shader->SetUniformMat4f("u_Model", modelTransform);
		renderer.DrawArray(*m_VAO, *m_Shader);
	}

}

void scene::Camera3DScene::OnImGuiRender()
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

void scene::Camera3DScene::SetCallbacks()
{
	glfwSetWindowUserPointer(m_Window, this);

	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		Camera3DScene* scene = static_cast<Camera3DScene*>(glfwGetWindowUserPointer(window));

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			scene->m_LastMouseX = x;
			scene->m_LastMouseY = y;
			scene->m_MouseHeld = true;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
			scene->m_MouseHeld = false;
		});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
		Camera3DScene* scene = static_cast<Camera3DScene*>(glfwGetWindowUserPointer(window));

		if (!scene->m_MouseHeld) return;

		double offsetX = xpos - scene->m_LastMouseX;
		double offsetY = scene->m_LastMouseY - ypos;

		scene->m_LastMouseX = xpos;
		scene->m_LastMouseY = ypos;

		scene->m_Camera->ProcessMouseMovement(offsetX, offsetY);
		});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
		Camera3DScene* scene = static_cast<Camera3DScene*>(glfwGetWindowUserPointer(window));
		scene->m_Camera->ProcessMouseScroll((float)yOffset);
		});

	m_CallbacksSet = true;
}


