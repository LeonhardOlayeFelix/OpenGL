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

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();

	m_LightVAO = std::make_unique<VertexArray>();
	m_LightVAO->Bind();
	m_LightVAO->RecordVBOLayout(*m_VBO, layout);
	m_LightVAO->Unbind();

	m_Texture = std::make_unique<Texture>("res/textures/MinecraftLamp.png");
	m_Texture->Bind(0);

	m_Texture2 = std::make_unique<Texture>("res/textures/container2.png");
	m_Texture2->Bind(1);

	m_Texture3 = std::make_unique<Texture>("res/textures/container2_specular.png");
	m_Texture3->Bind(2);

	m_Texture4 = std::make_unique<Texture>("res/textures/matrix.jpg");
	m_Texture4->Bind(3);

	m_PhongShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneObjectShaderPhong.shader");
	m_GouraudShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneObjectShaderGouraud.shader");

	m_LampShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneLampShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);

	m_ObjectShader = m_PhongShader.get();
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
	Renderer renderer;

	if (m_AutoMove) {
		m_LightPosition.x = m_ObjectTranslate.x + 3.0f * cos(glfwGetTime());
		m_LightPosition.y = m_ObjectTranslate.y + 5.0f * sin(glfwGetTime()) * cos(glfwGetTime());
		m_LightPosition.z = m_ObjectTranslate.z + 3.0f * sin(glfwGetTime());
		m_ObjectRotate.x = fmod(25 * glfwGetTime(), 360.0);
		m_ObjectRotate.y = fmod(25 * glfwGetTime(), 360.0);
		m_ObjectRotate.z = fmod(25 * glfwGetTime(), 360.0);
	} 

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, m_ObjectTranslate);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.y), glm::vec3(0.0, 1.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.z), glm::vec3(0.0, 0.0, 1.0));
	modelMatrix = glm::scale(modelMatrix, m_ObjectScale);

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

	m_VAO->Bind();
	m_ObjectShader->Bind();
	m_Texture2->Bind(1);
	m_Texture3->Bind(2);
	m_Texture4->Bind(3);
	m_ObjectShader->SetUniformMat4f("u_Model", modelMatrix);
	m_ObjectShader->SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_ObjectShader->SetUniformMat3f("u_Normal", normalMatrix);
	m_ObjectShader->SetUniform3f("u_ViewPosition", m_Camera->Position);
	m_ObjectShader->SetUniform3f("u_Light.position", m_LightPosition);
	m_ObjectShader->SetUniform3f("u_Light.ambient", m_LightAmbient);
	m_ObjectShader->SetUniform3f("u_Light.diffuse", m_LightDiffuse);
	m_ObjectShader->SetUniform3f("u_Light.specular", m_LightSpecular);
	m_ObjectShader->SetUniform1i("u_Material.diffuse", 1);
	m_ObjectShader->SetUniform1i("u_Material.specular", 2);
	m_ObjectShader->SetUniform1i("u_Material.emission", 3);
	m_ObjectShader->SetUniform1f("u_Material.shininess", m_Shininess);
	m_ObjectShader->SetUniform1f("u_Kc", m_Kc);
	m_ObjectShader->SetUniform1f("u_Kl", m_Kl);
	m_ObjectShader->SetUniform1f("u_Kq", m_Kq);
	renderer.DrawArray(*m_VAO, *m_ObjectShader);

	glm::mat4 lightModelMatrix = glm::translate(glm::mat4(1.0f), m_LightPosition);
	if (m_AutoMove) lightModelMatrix = glm::rotate(lightModelMatrix, (float)glm::radians(100 * glfwGetTime()), glm::vec3(1.0, 1.0, 1.0));
	
	m_Texture->Bind(0);
	m_LightVAO->Bind();
	m_LampShader->Bind();
	m_LampShader->SetUniformMat4f("u_View", viewMatrix);
	m_LampShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_LampShader->SetUniformMat4f("u_Model", lightModelMatrix);
	m_LampShader->SetUniform1i("u_Texture", 0);
	renderer.DrawArray(*m_LightVAO, *m_LampShader);

	m_Camera->UpdateCameraVectors();
}

void scene::LightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Object Properties"))
	{
		ImGui::SliderFloat3("Specular_o", glm::value_ptr(m_ObjectSpecular), 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess_o", &m_Shininess, 0.0f, 200.0f);
	}

	if (ImGui::CollapsingHeader("Lamp Properties"))
	{
		ImGui::SliderFloat3("Ambience", glm::value_ptr(m_LightAmbient), 0.0f, 1.0f);
		ImGui::SliderFloat3("Diffuse", glm::value_ptr(m_LightDiffuse), 0.0f, 1.0f);
		ImGui::SliderFloat3("Specular", glm::value_ptr(m_LightSpecular), 0.0f, 1.0f);
		ImGui::SliderFloat3("Position", glm::value_ptr(m_LightPosition), -10.0f, 10.0f);
		ImGui::Checkbox("Auto Move Light", &m_AutoMove);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

}


