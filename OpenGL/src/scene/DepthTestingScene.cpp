#include "DepthTestingScene.h"
#include "vendor/imgui/imgui.h"


scene::DepthTestingScene::DepthTestingScene()
{
	DoPreviousInit();

}

scene::DepthTestingScene::~DepthTestingScene()
{
}

void scene::DepthTestingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::DepthTestingScene::OnRender()
{
	Renderer renderer;
	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();

	m_ObjectShader->Bind();
	m_ObjectShader->SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader->SetUniformMat4f("u_Proj", perspectiveMatrix);

	//Stone
	glm::mat4 modelMatrixStone = glm::scale(glm::mat4(1), glm::vec3(4));
	m_ObjectShader->SetUniformMat4f("u_Model", modelMatrixStone);
	m_StoneTexture->Bind(0);
	m_ObjectShader->SetUniform1i("u_Texture", 0);
	renderer.DrawArray(*m_VAO, *m_ObjectShader);


	//Lamp
	glm::mat4 modelMatrixLamp = glm::translate(glm::mat4(1), glm::vec3(0, 2, 0));
	m_ObjectShader->SetUniformMat4f("u_Model", modelMatrixLamp);
	m_LampTexture->Bind(1);
	m_ObjectShader->SetUniform1i("u_Texture", 1);

	renderer.DrawArray(*m_VAO, *m_ObjectShader);

	m_Camera->UpdateCameraVectors();
}

void scene::DepthTestingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Depth Settings"))
	{

	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::DepthTestingScene::DoPreviousInit()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
					   
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
					   
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
					   
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
					   
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
					   
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);


	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();

	m_StoneTexture = std::make_unique<Texture>("res/textures/MinecraftStone.jpg");
	m_StoneTexture->Bind(0);
	m_LampTexture = std::make_unique<Texture>("res/textures/MinecraftLamp.png");
	m_LampTexture->Bind(1);

	m_ObjectShader = std::make_unique<ShaderProgram>("res/shaders/DepthTestingSceneShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);
}
