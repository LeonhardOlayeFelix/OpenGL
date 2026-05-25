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

	m_PhongShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneObjectShaderPhong.shader");
	m_GouraudShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneObjectShaderGouraud.shader");

	m_LampShader = std::make_unique<ShaderProgram>("res/shaders/LightSceneLampShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0, 0.0f);
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
	m_Texture->Bind(0);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, m_ObjectTranslate);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.x), glm::vec3(1.0, 0.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.y), glm::vec3(0.0, 1.0, 0.0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_ObjectRotate.z), glm::vec3(0.0, 0.0, 1.0));
	modelMatrix = glm::scale(modelMatrix, m_ObjectScale);

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

	Renderer renderer;
	m_ObjectShader = (m_ShadingModel == 0) ? m_PhongShader.get() : m_GouraudShader.get();

	m_VAO->Bind();
	m_ObjectShader->Bind();
	m_ObjectShader->SetUniformMat4f("u_Model", modelMatrix);
	m_ObjectShader->SetUniformMat4f("u_View", viewMatrix);
	m_ObjectShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_ObjectShader->SetUniformMat3f("u_Normal", normalMatrix);
	m_ObjectShader->SetUniform3f("u_Albedo", m_ObjectAlbedo);
	m_ObjectShader->SetUniform3f("u_LightIntensity", m_LightIntensity);
	m_ObjectShader->SetUniform3f("u_LightPosition", m_LightPosition);
	m_ObjectShader->SetUniform3f("u_ViewPosition", m_Camera->Position);
	m_ObjectShader->SetUniform1f("u_Ka", m_Ka);
	m_ObjectShader->SetUniform1f("u_Kd", m_Kd);
	m_ObjectShader->SetUniform1f("u_Ks", m_Ks);
	m_ObjectShader->SetUniform1f("u_Shininess", m_Shininess);
	renderer.DrawArray(*m_VAO, *m_ObjectShader);


	m_LightVAO->Bind();
	m_LampShader->Bind();
	m_LampShader->SetUniformMat4f("u_View", viewMatrix);
	m_LampShader->SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_LampShader->SetUniformMat4f("u_Model", glm::translate(glm::mat4(1.0f), m_LightPosition));
	m_LampShader->SetUniform3f("u_LightIntensity", m_LightIntensity);
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
		ImGui::ColorPicker3("Albedo", glm::value_ptr(m_ObjectAlbedo), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);
		ImGui::SliderFloat3("Translate", glm::value_ptr(m_ObjectTranslate), -5.0f, 5.0f);
		ImGui::SliderFloat3("Rotate", glm::value_ptr(m_ObjectRotate), 0.0, 360.0);
		ImGui::SliderFloat3("Scale", glm::value_ptr(m_ObjectScale), 0.5f, 3.0f);
		const char* shadingModels[] = { "Phong", "Gouraud" };
		ImGui::Combo("Shading Model", &m_ShadingModel, shadingModels, IM_ARRAYSIZE(shadingModels));

		if (ImGui::CollapsingHeader("Reflection Parameters"))
		{
			ImGui::SliderFloat("Ka (Ambient)", &m_Ka, 0.0f, 1.0f);
			ImGui::SliderFloat("Kd (Diffuse)", &m_Kd, 0.0f, 1.0f);
			ImGui::SliderFloat("Ks (Specular)", &m_Ks, 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 1.0f);
		}
		

	}

	if (ImGui::CollapsingHeader("Lamp Properties"))
	{
		ImGui::SliderFloat3("Position", glm::value_ptr(m_LightPosition), -5.0f, 5.0f);
		ImGui::SliderFloat3("Intensity", glm::value_ptr(m_LightIntensity), 0.0f, 1.0f);
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

}


