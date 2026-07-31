#include "BlendingScene.h"
#include "vendor/imgui/imgui.h"


scene::BlendingScene::BlendingScene()
{
	DoPreviousInit();

	float quadVertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  1.0f
	};

	m_Vegetation.push_back(glm::vec3(-1.5f, 2.5f, -0.48f));
	m_Vegetation.push_back(glm::vec3(1.5f, 2.5f, 0.51f));
	m_Vegetation.push_back(glm::vec3(0.0f, 2.5f, 0.7f));
	m_Vegetation.push_back(glm::vec3(-0.3f, 2.5f, -2.3f));
	m_Vegetation.push_back(glm::vec3(0.5f, 2.5f, -0.6f));

	m_Windows.push_back(glm::vec3(-1.5f, 2.5f, -0.48f));
	m_Windows.push_back(glm::vec3(1.5f, 2.5f, 0.51f));
	m_Windows.push_back(glm::vec3(0.0f, 2.5f, 0.7f));
	m_Windows.push_back(glm::vec3(-0.3f, 2.5f, -2.3f));
	m_Windows.push_back(glm::vec3(0.5f, 2.5f, -0.6f));

	//Sort transparent windows in order of distance to camera, farthest first.
	glm::vec3 camPos = m_Camera->Position;
	std::sort(m_Windows.begin(), m_Windows.end(),
		[&camPos](const glm::vec3& a, const glm::vec3& b) {
			float distA = glm::length(camPos - a);
			float distB = glm::length(camPos - b);
			return distA > distB;
		});

	m_VAO2 = std::make_unique<VertexArray>();
	m_VAO2->Bind();
	m_VBO2 = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO2->RecordVBOLayout(*m_VBO2, layout);

	m_GrassTexture = std::make_unique<Texture>("res/textures/Grass.png");
	m_WindowTexture = std::make_unique<Texture>("res/textures/window.png");
}

scene::BlendingScene::~BlendingScene()
{
}

void scene::BlendingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::BlendingScene::OnRender()
{
	Renderer renderer;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();

	m_BlockShader->Bind();
	m_BlockShader->SetUniformMat4f("u_View", viewMatrix);
	m_BlockShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_StoneTexture->Bind(0);
	m_LampTexture->Bind(1);
	m_GrassTexture->Bind(2);
	m_WindowTexture->Bind(3);
	m_VAO->Bind();

	//Stone floor
	m_BlockShader->SetUniform1i("u_Texture", 0);

	glm::mat4 modelMatrixStone = glm::scale(glm::mat4(1), glm::vec3(8, 4, 8));
	m_BlockShader->SetUniformMat4f("u_Model", modelMatrixStone);
	renderer.DrawArray(*m_VAO, *m_BlockShader);

	//Lamp 1
	m_BlockShader->SetUniform1i("u_Texture", 1);

	glm::mat4 modelMatrixLamp1 = glm::translate(glm::mat4(1), glm::vec3(1, 2.5, -1));
	m_BlockShader->SetUniformMat4f("u_Model", modelMatrixLamp1);
	renderer.DrawArray(*m_VAO, *m_BlockShader);

	//Lamp 2
	glm::mat4 modelMatrixLamp2 = glm::translate(glm::mat4(1), glm::vec3(-1, 2.5, 1));
	m_BlockShader->SetUniformMat4f("u_Model", modelMatrixLamp2);
	renderer.DrawArray(*m_VAO, *m_BlockShader);

	//Grass
	m_BlockShader->SetUniform1i("u_Texture", 2);
	glm::mat4 grassModel;
	for (unsigned int i = 0; i < m_Vegetation.size(); i++) 
	{
		grassModel = glm::mat4(1.0);
		grassModel = glm::translate(grassModel, m_Vegetation[i]);
		m_BlockShader->SetUniformMat4f("u_Model", grassModel);
		renderer.DrawArray(*m_VAO2, *m_BlockShader);
	}

	//Windows
	m_BlockShader->SetUniform1i("u_Texture", 3);
	glm::mat4 windowModel;
	for (unsigned int i = 0; i < m_Windows.size(); i++)
	{
		windowModel = glm::mat4(1.0);
		windowModel = glm::translate(windowModel, m_Windows[i]);
		m_BlockShader->SetUniformMat4f("u_Model", windowModel);
		renderer.DrawArray(*m_VAO2, *m_BlockShader);
	}

	m_Camera->UpdateCameraVectors();
}

void scene::BlendingScene::OnImGuiRender()
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

void scene::BlendingScene::DoPreviousInit()
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





	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();


	m_StoneTexture = std::make_unique<Texture>("res/textures/MinecraftStone.jpg");
	m_StoneTexture->Bind(0);
	m_LampTexture = std::make_unique<Texture>("res/textures/MinecraftLamp.png");
	m_LampTexture->Bind(1);

	m_BlockShader = std::make_unique<ShaderProgram>("res/shaders/BlendingSceneShader.shader");
	

	m_Camera = std::make_unique<Camera>(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;
}
