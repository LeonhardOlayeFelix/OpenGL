#include "AdvancedLightingScene.h"
#include "vendor/imgui/imgui.h"

scene::AdvancedLightingScene::AdvancedLightingScene()
{
	DoPreviousInit();
	m_PointLight = PointLight(glm::vec3(0.0), glm::vec3(0.05), glm::vec3(0.1), glm::vec3(-5, 1, 0), 1, 0.09f, 0.1f);
	m_PointLight2 = PointLight(glm::vec3(0.0), glm::vec3(0.1), glm::vec3(0.2), glm::vec3(0, 1, 0), 1, 0.09f, 0.1f);
	m_PointLight3 = PointLight(glm::vec3(0.0), glm::vec3(0.2), glm::vec3(0.3), glm::vec3(5, 1, 0), 1, 0.09f, 0.1f);
	m_PointLight4 = PointLight(glm::vec3(0.0), glm::vec3(0.4), glm::vec3(0.4), glm::vec3(10, 1, 0), 1, 0.09f, 0.1f);
}

scene::AdvancedLightingScene::~AdvancedLightingScene()
{
}

void scene::AdvancedLightingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::AdvancedLightingScene::OnRender()
{
	Renderer renderer;
	m_UBO->SetData(glm::value_ptr(m_Camera->GetViewMatrix()), sizeof(glm::mat4), sizeof(glm::mat4));


	m_Shader->SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(50, 1, 50)));
	m_Shader->SetUniform3f("u_ViewPosition", m_Camera->Position);
	m_Shader->SetUniform1i("u_WoodMaterial.texture_diffuse1", 0);
	m_Shader->SetUniform1i("u_WoodMaterial.texture_specular1", 1);
	m_Shader->SetUniform1f("u_WoodMaterial.shininess", m_Shininess);
	m_Shader->SetUniform1f("u_WoodMaterial.blinn", m_IsBlinn);

	m_Shader->SetUniform1PointLight("u_PointLight", m_PointLight);
	m_Shader->SetUniform1PointLight("u_PointLight2", m_PointLight2);
	m_Shader->SetUniform1PointLight("u_PointLight3", m_PointLight3);
	m_Shader->SetUniform1PointLight("u_PointLight4", m_PointLight4);

	renderer.DrawArray(*m_VAO, *m_Shader);


	m_Camera->UpdateCameraVectors();
}

void scene::AdvancedLightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Material Settings"))
	{
		ImGui::SliderFloat("Shininess", &m_Shininess, 1, 512);
		ImGui::Checkbox("Use blinn", &m_IsBlinn);
	}
	if (ImGui::CollapsingHeader("Light Settings"))
	{
		ImGui::SliderFloat("X1", &m_PointLight.Position.x, -10, 10);
		ImGui::SliderFloat("X2", &m_PointLight2.Position.x, -10, 10);
		ImGui::SliderFloat("X3", &m_PointLight3.Position.x, -10, 10);
		ImGui::SliderFloat("X4", &m_PointLight4.Position.x, -10, 10);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::AdvancedLightingScene::DoPreviousInit()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 15.0, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 15.0, 15.0,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 15.0, 15.0,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 15.0,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  15.0, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  15.0, 15.0,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  15.0, 15.0,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 15.0,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 15.0, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 15.0, 15.0,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 15.0,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 15.0,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 15.0, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 15.0, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 15.0, 15.0,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 15.0,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 15.0,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 15.0, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 15.0,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 15.0, 15.0,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 15.0, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 15.0, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 15.0,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 15.0,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 15.0, 15.0,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 15.0, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 15.0, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 15.0
	};

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_Camera = std::make_unique<Camera>(glm::vec3(3, 2, 3), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;

	m_WoodDiffuse = std::make_unique<Texture>("res/textures/WoodTiles.jpg", GL_SRGB8_ALPHA8);
	m_WoodDiffuse->Bind(0);

	m_WoodSpecular = std::make_unique<Texture>("res/textures/White.jpg");
	m_WoodSpecular->Bind(1);

	

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/BlinnPhongLightingShader.shader");
	m_Shader->SetUniformBlockBinding("Matrices", 0);

	m_UBO = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4));
	m_UBO->BindToPoint(0);
	m_UBO->SetData(glm::value_ptr(m_Camera->GetPerspectiveMatrix()), sizeof(glm::mat4), 0);
}
