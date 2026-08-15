#include "MeshLoadingScene.h"
#include "vendor/imgui/imgui.h"


scene::MeshLoadingScene::MeshLoadingScene()
{
	DoPreviousInit();

	m_Model = Model("res/models/backpack/backpack.obj");
}

scene::MeshLoadingScene::~MeshLoadingScene()
{
}

void scene::MeshLoadingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::MeshLoadingScene::OnRender()
{
	Renderer renderer;
	glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera.GetPerspectiveMatrix();
	glm::mat4 lightModelMatrix = glm::translate(glm::mat4(1.0f), m_PointLightPosition);

	m_LightVAO.Bind();
	m_LampShader.Bind();
	m_LampShader.SetUniformMat4f("u_View", viewMatrix);
	m_LampShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_LampShader.SetUniformMat4f("u_Model", lightModelMatrix);
	m_LampShader.SetUniform3f("u_Diffuse", m_PointLightDiffuse);
	m_Texture.Bind(0);
	m_LampShader.SetUniform1i("u_Texture", 0);
	renderer.DrawArray(m_LightVAO, m_LampShader);

	glm::mat4 bagModelMatrix = glm::mat4(1.0f);
	glm::mat3 bagNormalMatrix = glm::mat3(glm::transpose(glm::inverse(bagModelMatrix)));

	m_ObjectShader.Bind();
	m_ObjectShader.SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_ObjectShader.SetUniformMat4f("u_Model", bagModelMatrix);
	m_ObjectShader.SetUniformMat3f("u_Normal", bagNormalMatrix);
	m_ObjectShader.SetUniform3f("u_DirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
	m_ObjectShader.SetUniform3f("u_DirLight.ambient", glm::vec3(0.1f));
	m_ObjectShader.SetUniform3f("u_DirLight.diffuse", glm::vec3(0.3f));
	m_ObjectShader.SetUniform3f("u_DirLight.specular", glm::vec3(0.5f));
	m_ObjectShader.SetUniform3f("u_PointLight.position", m_PointLightPosition);
	m_ObjectShader.SetUniform3f("u_PointLight.ambient", m_PointLightAmbient);
	m_ObjectShader.SetUniform3f("u_PointLight.diffuse", m_PointLightDiffuse);
	m_ObjectShader.SetUniform3f("u_PointLight.specular", m_PointLightSpecular);
	m_ObjectShader.SetUniform1f("u_PointLight.Kc", m_PointLightKc);
	m_ObjectShader.SetUniform1f("u_PointLight.Kl", m_PointLightKl);
	m_ObjectShader.SetUniform1f("u_PointLight.Kq", m_PointLightKq);
	
	
	
	
	m_ObjectShader.SetUniform3f("u_ViewPosition", m_Camera.Position);
	m_ObjectShader.SetUniform1f("u_Material.shininess", 32.0f);

	m_Model.Draw(m_ObjectShader);

	m_Camera.UpdateCameraVectors();
}

void scene::MeshLoadingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Point Light"))
	{
		ImGui::SliderFloat3("Position", glm::value_ptr(m_PointLightPosition), -10.0f, 10.0f);
		ImGui::SliderFloat3("Ambient", glm::value_ptr(m_PointLightAmbient), 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(m_PointLightDiffuse), 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", glm::value_ptr(m_PointLightSpecular), 0.0f, 1.0f);
		ImGui::SliderFloat("Kc", &m_PointLightKc, -1.0f, 1.0f);
		ImGui::SliderFloat("Kl", &m_PointLightKl, -1.0f, 1.0f);
		ImGui::SliderFloat("Kq", &m_PointLightKq, -1.0f, 1.0f);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::MeshLoadingScene::DoPreviousInit()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_VBO = VertexBuffer(vertices, sizeof(vertices));

	m_LightVAO = VertexArray();
	m_LightVAO.Bind();
	m_LightVAO.RecordVBOLayout(m_VBO, layout);
	m_LightVAO.Unbind();

	m_Texture = Texture("res/textures/MinecraftLamp.png");
	m_Texture.Bind(0);

	m_ObjectShader = ShaderProgram("res/shaders/MeshLoadingSceneShader.shader");
	m_LampShader = ShaderProgram("res/shaders/LightSceneLampShader.shader");

	m_Camera = Camera(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);
}
