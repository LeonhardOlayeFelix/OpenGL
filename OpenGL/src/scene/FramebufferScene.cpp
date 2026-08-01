#include "FramebufferScene.h"
#include "vendor/imgui/imgui.h"
#include <iostream>

scene::FramebufferScene::FramebufferScene()
{
	DoPreviousInit();

	m_FBO2 = std::make_unique<FrameBuffer>(960, 540);
	m_FBO2->AddColorAttachment();
	m_FBO2->AddDepthStencilAttachment();
	m_FBO2->Validate();
}

scene::FramebufferScene::~FramebufferScene()
{
}

void scene::FramebufferScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::FramebufferScene::OnRender()
{
	Renderer renderer;

	m_FBO2->Bind();
	renderer.Clear();
	glEnable(GL_DEPTH_TEST);

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();

	m_StoneTexture->Bind(0);
	m_LampTexture->Bind(1);

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_View", viewMatrix);
	m_Shader->SetUniformMat4f("u_Proj", perspectiveMatrix);

	//Stone floor
	m_Shader->SetUniform1i("u_Texture", 0);

	glm::mat4 modelMatrixStone = glm::scale(glm::mat4(1), glm::vec3(4));
	m_Shader->SetUniformMat4f("u_Model", modelMatrixStone);
	renderer.DrawArray(*m_VAO, *m_Shader);

	//Lamp 1
	m_Shader->SetUniform1i("u_Texture", 1);

	glm::mat4 modelMatrixLamp1 = glm::translate(glm::mat4(1), glm::vec3(1, 2.5, -1));
	m_Shader->SetUniformMat4f("u_Model", modelMatrixLamp1);
	renderer.DrawArray(*m_VAO, *m_Shader);

	//Lamp 2
	glm::mat4 modelMatrixLamp2 = glm::translate(glm::mat4(1), glm::vec3(-1, 2.5, 1));
	m_Shader->SetUniformMat4f("u_Model", modelMatrixLamp2);
	renderer.DrawArray(*m_VAO, *m_Shader);

	m_FBO2->ColorAttachmentTexture->Bind();
	m_FBO2->Unbind();

	//Back to default frame buffer -> paste view texture onto screen quad
	renderer.Clear();
	m_ScreenShader->Bind();
	m_ScreenShader->SetUniform1f("u_Offset", m_KernelOffset);
	m_ScreenShader->SetUniform1fv("u_Kernel", 9, m_Kernel);
	glDisable(GL_DEPTH_TEST);
	renderer.DrawArray(*m_VAO2, *m_ScreenShader);


	m_Camera->UpdateCameraVectors();

}

void scene::FramebufferScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Kernel Settings"))
	{
		ImGui::SliderFloat("Kernel Offset", &m_KernelOffset, 0.001f, 1.0f);

		ImGui::Text("Convolution Kernel");
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				int idx = row * 3 + col;
				ImGui::PushID(idx);
				ImGui::SetNextItemWidth(60.0f);
				ImGui::DragFloat("##kernelCell", &m_Kernel[idx], 0.1f, -20.0f, 20.0f);
				ImGui::PopID();
				if (col < 2) ImGui::SameLine();
			}
		}

		if (ImGui::Button("Reset to Sharpen"))
		{
			float sharpen[9] = { -1,-1,-1, -1,9,-1, -1,-1,-1 };
			memcpy(m_Kernel, sharpen, sizeof(m_Kernel));
		}
		ImGui::SameLine();
		if (ImGui::Button("Identity"))
		{
			float identity[9] = { 0,0,0, 0,1,0, 0,0,0 };
			memcpy(m_Kernel, identity, sizeof(m_Kernel));
		}
		ImGui::SameLine();
		if (ImGui::Button("Blur"))
		{
			float blur[9] = { 1,1,1, 1,1,1, 1,1,1 };
			for (int i = 0; i < 9; i++) blur[i] /= 16.0f;
			memcpy(m_Kernel, blur, sizeof(m_Kernel));
		}
		ImGui::SameLine();
		if (ImGui::Button("Edge Detect"))
		{
			float edge[9] = { 1,1,1, 1,-8,1, 1,1,1 };
			memcpy(m_Kernel, edge, sizeof(m_Kernel));
		}
	}

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::FramebufferScene::DoPreviousInit()
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

	m_VAO2 = std::make_unique<VertexArray>();
	m_VAO2->Bind();
	m_VBO2 = std::make_unique<VertexBuffer>(ndcQuadVertices, sizeof(ndcQuadVertices));
	m_VAO2->RecordVBOLayout(*m_VBO2, layout2);
	m_VAO2->Unbind();

	m_StoneTexture = std::make_unique<Texture>("res/textures/MinecraftStone.jpg");
	m_StoneTexture->Bind(0);
	m_LampTexture = std::make_unique<Texture>("res/textures/MinecraftLamp.png");
	m_LampTexture->Bind(1);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/BlendingSceneShader.shader");
	m_ScreenShader = std::make_unique<ShaderProgram>("res/shaders/FramebufferShader.shader");

	m_Camera = std::make_unique<Camera>(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;
}
