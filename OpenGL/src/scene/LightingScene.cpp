#include "LightingScene.h"
#include "vendor/imgui/imgui.h"

scene::LightingScene::LightingScene()
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

	m_VAO = VertexArray();
	m_VAO.Bind();
	m_VAO.RecordVBOLayout(m_VBO, layout);
	m_VAO.Unbind();

	m_LightVAO = VertexArray();
	m_LightVAO.Bind();
	m_LightVAO.RecordVBOLayout(m_VBO, layout);
	m_LightVAO.Unbind();

	m_Texture = Texture("res/textures/MinecraftLamp.png");
	m_Texture.Bind(0);
	m_Texture2 = Texture("res/textures/container2.png");
	m_Texture2.Bind(1);
	m_Texture3 = Texture("res/textures/container2_specular.png");
	m_Texture3.Bind(2);

	m_ObjectShader = ShaderProgram("res/shaders/LightSceneObjectShaderPhong.shader");
	m_LampShader = ShaderProgram("res/shaders/LightSceneLampShader.shader");

	m_Camera = Camera(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);

	m_PointLights[0].position = glm::vec3(0.7f, 0.2f, 2.0f);
	m_PointLights[1].position = glm::vec3(2.3f, -3.3f, -4.0f);
	m_PointLights[2].position = glm::vec3(-4.0f, 2.0f, -12.0f);
	m_PointLights[3].position = glm::vec3(0.0f, 0.0f, -3.0f);
}

scene::LightingScene::~LightingScene()
{
}

void scene::LightingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

static void SetDirLightUniforms(ShaderProgram& shader, const scene::DirLightData& light)
{
	shader.SetUniform3f("u_DirLight.direction", light.direction);
	shader.SetUniform3f("u_DirLight.ambient", light.ambient);
	shader.SetUniform3f("u_DirLight.diffuse", light.diffuse);
	shader.SetUniform3f("u_DirLight.specular", light.specular);
}
static void SetSpotLightUniforms(ShaderProgram& shader, const scene::SpotLightData& light, const Camera& camera)
{
	shader.SetUniform3f("u_SpotLight.position", camera.Position);
	shader.SetUniform3f("u_SpotLight.direction", camera.Front);
	shader.SetUniform3f("u_SpotLight.ambient", light.ambient);
	shader.SetUniform3f("u_SpotLight.diffuse", light.diffuse);
	shader.SetUniform3f("u_SpotLight.specular", light.specular);
	shader.SetUniform1f("u_SpotLight.cutOff", glm::cos(glm::radians(light.cutOff)));
	shader.SetUniform1f("u_SpotLight.outerCutOff", glm::cos(glm::radians(light.outerCutOff)));
	shader.SetUniform1f("u_SpotLight.Kc", light.Kc);
	shader.SetUniform1f("u_SpotLight.Kl", light.Kl);
	shader.SetUniform1f("u_SpotLight.Kq", light.Kq);
}

static void SetPointLightUniforms(ShaderProgram& shader, int index, const scene::PointLightData& light)
{
	std::string base = "u_PointLights[" + std::to_string(index) + "].";
	shader.SetUniform3f((base + "position").c_str(), light.position);
	shader.SetUniform3f((base + "ambient").c_str(), light.ambient);
	shader.SetUniform3f((base + "diffuse").c_str(), light.diffuse);
	shader.SetUniform3f((base + "specular").c_str(), light.specular);
	shader.SetUniform1f((base + "Kc").c_str(), light.Kc);
	shader.SetUniform1f((base + "Kl").c_str(), light.Kl);
	shader.SetUniform1f((base + "Kq").c_str(), light.Kq);
}

void scene::LightingScene::OnRender()
{
	Renderer renderer;

	if (m_AutoMove) {
		m_PointLights[0].position.x = m_ObjectTranslate.x + 3.0f * cos(glfwGetTime());
		m_PointLights[0].position.y = m_ObjectTranslate.y + 5.0f * sin(glfwGetTime()) * cos(glfwGetTime());
		m_PointLights[0].position.z = m_ObjectTranslate.z + 3.0f * sin(glfwGetTime());
		m_ObjectRotate.x = fmod(25 * glfwGetTime(), 360.0);
		m_ObjectRotate.y = fmod(25 * glfwGetTime(), 360.0);
		m_ObjectRotate.z = fmod(25 * glfwGetTime(), 360.0);
	}

	glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera.GetPerspectiveMatrix();

	m_VAO.Bind();
	m_ObjectShader.Bind();
	m_Texture2.Bind(1);
	m_Texture3.Bind(2);

	m_ObjectShader.SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_ObjectShader.SetUniform3f("u_ViewPosition", m_Camera.Position);

	m_ObjectShader.SetUniform1i("u_Material.diffuse", 1);
	m_ObjectShader.SetUniform1i("u_Material.specular", 2);
	m_ObjectShader.SetUniform1f("u_Material.shininess", m_Shininess);

	SetDirLightUniforms(m_ObjectShader, m_DirLight);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		SetPointLightUniforms(m_ObjectShader, i, m_PointLights[i]);

	SetSpotLightUniforms(m_ObjectShader, m_SpotLight, m_Camera);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,   0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f,  -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,  -7.5f),
		glm::vec3(1.3f, -2.0f,  -2.5f),
		glm::vec3(1.5f,  2.0f,  -2.5f),
		glm::vec3(1.5f,  0.2f,  -1.5f),
		glm::vec3(-1.3f,  1.0f,  -1.5f)
	};

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
		m_ObjectShader.SetUniformMat4f("u_Model", model);

		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
		m_ObjectShader.SetUniformMat3f("u_Normal", normalMatrix);

		renderer.DrawArray(m_VAO, m_ObjectShader);
	}

	m_Texture.Bind(0);
	m_LightVAO.Bind();
	m_LampShader.Bind();

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		glm::mat4 lightModelMatrix = glm::translate(glm::mat4(1.0f), m_PointLights[i].position);
		if (m_AutoMove)
			lightModelMatrix = glm::rotate(lightModelMatrix, (float)glm::radians(100 * glfwGetTime()), glm::vec3(1.0, 1.0, 1.0));
		lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

		m_LampShader.SetUniformMat4f("u_View", viewMatrix);
		m_LampShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
		m_LampShader.SetUniformMat4f("u_Model", lightModelMatrix);
		m_LampShader.SetUniform3f("u_Diffuse", m_PointLights[i].diffuse);
		m_LampShader.SetUniform1i("u_Texture", 0);

		renderer.DrawArray(m_LightVAO, m_LampShader);
	}
	

	m_Camera.UpdateCameraVectors();
}

void scene::LightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Object Properties"))
	{
		ImGui::SliderFloat3("Specular", glm::value_ptr(m_ObjectSpecular), 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 200.0f);
	}

	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::SliderFloat3("Dir Direction", glm::value_ptr(m_DirLight.direction), -1.0f, 1.0f);
		ImGui::SliderFloat3("Dir Ambient", glm::value_ptr(m_DirLight.ambient), 0.0f, 1.0f);
		ImGui::SliderFloat3("Dir Diffuse", glm::value_ptr(m_DirLight.diffuse), 0.0f, 1.0f);
		ImGui::SliderFloat3("Dir Specular", glm::value_ptr(m_DirLight.specular), 0.0f, 1.0f);
	}

	if (ImGui::CollapsingHeader("Point Lights"))
	{
		for (int i = 0; i < NR_POINT_LIGHTS; i++)
		{
			std::string label = "Point Light " + std::to_string(i);
			if (ImGui::TreeNode(label.c_str()))
			{
				ImGui::SliderFloat3("Position", glm::value_ptr(m_PointLights[i].position), -15.0f, 15.0f);
				ImGui::SliderFloat3("Ambient", glm::value_ptr(m_PointLights[i].ambient), 0.0f, 1.0f);
				ImGui::SliderFloat3("Diffuse", glm::value_ptr(m_PointLights[i].diffuse), 0.0f, 1.0f);
				ImGui::SliderFloat3("Specular", glm::value_ptr(m_PointLights[i].specular), 0.0f, 1.0f);
				ImGui::SliderFloat("Kc", &m_PointLights[i].Kc, 0.0f, 2.0f);
				ImGui::SliderFloat("Kl", &m_PointLights[i].Kl, 0.0f, 1.0f);
				ImGui::SliderFloat("Kq", &m_PointLights[i].Kq, 0.0f, 1.0f);
				ImGui::TreePop();
			}
		}
		ImGui::Checkbox("Auto Move Light 0", &m_AutoMove);
	}

	if (ImGui::CollapsingHeader("Spot Light"))
	{
		ImGui::SliderFloat3("Spot Ambient", glm::value_ptr(m_SpotLight.ambient), 0.0f, 1.0f);
		ImGui::SliderFloat3("Spot Diffuse", glm::value_ptr(m_SpotLight.diffuse), 0.0f, 1.0f);
		ImGui::SliderFloat3("Spot Specular", glm::value_ptr(m_SpotLight.specular), 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Kc", &m_SpotLight.Kc, 0.0f, 2.0f);
		ImGui::SliderFloat("Spot Kl", &m_SpotLight.Kl, 0.0f, 1.0f);
		ImGui::SliderFloat("Spot Kq", &m_SpotLight.Kq, 0.0f, 1.0f);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}