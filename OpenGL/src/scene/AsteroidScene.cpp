#include "AsteroidScene.h"
#include <GLFW/glfw3.h>
#include "vendor/imgui/imgui.h"

scene::AsteroidScene::AsteroidScene()
{
	DoPreviousInit();

	RandomiseModels();

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/AsteroidSceneRockShader.shader"); 
	m_Planet = std::make_unique<Model>("res/models/planet/planet.obj");
	m_Rock   = std::make_unique<Model>("res/models/rock/rock.obj"); 
}


scene::AsteroidScene::~AsteroidScene()
{
}

void scene::AsteroidScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::AsteroidScene::OnRender()
{
	Renderer renderer;

	glm::mat4 view = m_Camera->GetViewMatrix();
	glm::mat4 proj = m_Camera->GetPerspectiveMatrix();

	//renderer.DrawElementsInstanced(*m_VAO, *m_Shader, NR_OBJECTS);

	glm::mat4 planetModel = glm::mat4(1.0);
	glm::mat3 planetNormal = glm::mat3(glm::transpose(glm::inverse(planetModel)));

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_Model", planetModel);
	m_Shader->SetUniformMat4f("u_View", view);
	m_Shader->SetUniformMat4f("u_Proj", proj);
	m_Shader->SetUniformMat3f("u_Normal", planetNormal);

	m_Planet->Draw(*m_Shader);

	RandomiseModels();

	for (unsigned int i = 0; i < NR_ASTEROIDS; i++) {

		glm::mat4 rockModel = m_ModelMatrices[i];
		glm::mat3 rockNormal = glm::mat3(glm::transpose(glm::inverse(rockModel)));

		m_Shader->SetUniformMat4f("u_Model", rockModel);
		m_Shader->SetUniformMat3f("u_Normal", rockNormal);

		m_Rock->Draw(*m_Shader);
	}

	m_Camera->UpdateCameraVectors();
}

void scene::AsteroidScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();


	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::AsteroidScene::DoPreviousInit()
{
	m_Camera = std::make_unique<Camera>(glm::vec3(-0.75f, 3.91f, -5.22), glm::vec3(0.0f, 1.0f, 0.0f), 57.0f, -40.0f);
}

void scene::AsteroidScene::RandomiseModels()
{
	srand(glfwGetTime() * 10);

	for (unsigned int i = 0; i < NR_ASTEROIDS; i++) {
		glm::mat4 model = glm::mat4(1.0f);

		//translation
		float angle = (float)i / float(NR_ASTEROIDS) * 360.0f;
		float displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float x = sin(angle) * m_Radius + displacement;
		displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * m_Offset * 100)) / 100.0f - m_Offset;
		float z = cos(angle) * m_Radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		//scale
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		//rotate about arbitrary axis
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		m_ModelMatrices[i] = model;
	}
}
