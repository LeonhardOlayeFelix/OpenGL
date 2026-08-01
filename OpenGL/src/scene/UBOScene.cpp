#include "UBOScene.h"
#include "vendor/imgui/imgui.h"
#include <iostream>

scene::UBOScene::UBOScene()
{
	DoPreviousInit();

	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(m_ShaderRed->GetID(), "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(m_ShaderGreen->GetID(), "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(m_ShaderBlue->GetID(), "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(m_ShaderYellow->GetID(), "Matrices");

	glUniformBlockBinding(m_ShaderRed->GetID(), uniformBlockIndexRed, 0);
	glUniformBlockBinding(m_ShaderGreen->GetID(), uniformBlockIndexGreen, 0);
	glUniformBlockBinding(m_ShaderBlue->GetID(), uniformBlockIndexBlue, 0);
	glUniformBlockBinding(m_ShaderYellow->GetID(), uniformBlockIndexYellow, 0);

	glGenBuffers(1, &m_UBOMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOMatrices, 0, 2 * sizeof(glm::mat4));

	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(perspectiveMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

scene::UBOScene::~UBOScene()
{
}

void scene::UBOScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::UBOScene::OnRender()
{
	Renderer renderer;
	glm::mat4 model = glm::mat4(1.0);

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	model = glm::translate(glm::mat4(1.0), glm::vec3(-1, 1, 0));
	m_ShaderRed->SetUniformMat4f("u_Model", model);
	renderer.DrawArray(*m_VAO, *m_ShaderRed);

	model = glm::translate(glm::mat4(1.0), glm::vec3(-1, -1, 0));
	m_ShaderBlue->SetUniformMat4f("u_Model", model);
	renderer.DrawArray(*m_VAO, *m_ShaderBlue);

	model = glm::translate(glm::mat4(1.0), glm::vec3(1, -1, 0));
	m_ShaderYellow->SetUniformMat4f("u_Model", model);
	renderer.DrawArray(*m_VAO, *m_ShaderYellow);

	model = glm::translate(glm::mat4(1.0), glm::vec3(1, 1, 0));
	m_ShaderGreen->SetUniformMat4f("u_Model", model);
	renderer.DrawArray(*m_VAO, *m_ShaderGreen);


	m_Camera->UpdateCameraVectors();

}

void scene::UBOScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	
	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::UBOScene::DoPreviousInit()
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

	float ndcQuadVertices[] = {
		// positions      // texcoords
		-1.0f,  1.0f,     0.0f, 1.0f,   // top-left
		-1.0f, -1.0f,     0.0f, 0.0f,   // bottom-left
		 1.0f, -1.0f,     1.0f, 0.0f,   // bottom-right

		-1.0f,  1.0f,     0.0f, 1.0f,   // top-left
		 1.0f, -1.0f,     1.0f, 0.0f,   // bottom-right
		 1.0f,  1.0f,     1.0f, 1.0f    // top-right
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	VertexBufferLayout layout2 ;
	layout2.Push<float>(2);
	layout2.Push<float>(2);


	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();
	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	m_VAO->RecordVBOLayout(*m_VBO, layout);
	m_VAO->Unbind();

	m_ShaderGreen = std::make_unique<ShaderProgram>("res/shaders/UBOSceneShaderGreen.shader");
	m_ShaderRed = std::make_unique<ShaderProgram>("res/shaders/UBOSceneShaderRed.shader");
	m_ShaderBlue = std::make_unique<ShaderProgram>("res/shaders/UBOSceneShaderBlue.shader");
	m_ShaderYellow = std::make_unique<ShaderProgram>("res/shaders/UBOSceneShaderYellow.shader");



	m_Camera = std::make_unique<Camera>(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;
}
