#include "FramebufferScene.h"
#include "vendor/imgui/imgui.h"
#include <iostream>

scene::FramebufferScene::FramebufferScene()
{
	DoPreviousInit();
	m_ResolveFBO = FrameBuffer(1920, 1080);
	m_ResolveFBO.AddAttachment(AttachmentTarget::Color, AttachmentStorage::Texture);
	m_ResolveFBO.Validate();

	m_MultisampleFBO = FrameBuffer(1920, 1080, 4);
	m_MultisampleFBO.AddAttachment(AttachmentTarget::Color, AttachmentStorage::Texture);
	m_MultisampleFBO.AddAttachment(AttachmentTarget::DepthStencil, AttachmentStorage::RenderBuffer);
	m_MultisampleFBO.Validate();
}

scene::FramebufferScene::~FramebufferScene()
{
}

void scene::FramebufferScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::FramebufferScene::OnRender()
{
	Renderer renderer;

	m_MultisampleFBO.Bind();
	renderer.Clear();
	glEnable(GL_DEPTH_TEST);

	glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera.GetPerspectiveMatrix();

	m_Shader.SetUniformMat4f("u_View", viewMatrix);
	m_Shader.SetUniformMat4f("u_Proj", perspectiveMatrix);

	m_StoneTexture.Bind(0);
	m_LampTexture.Bind(1);

	m_Shader.SetUniform1i("u_Texture", 0);
	m_Shader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(4)));
	renderer.DrawArray(m_VAO, m_Shader);
	m_Shader.SetUniform1i("u_Texture", 1);
	m_Shader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(1, 2.5, -1)));
	renderer.DrawArray(m_VAO, m_Shader);
	m_Shader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(-1, 2.5, 1)));
	renderer.DrawArray(m_VAO, m_Shader);

	//Blit the Multi sampled frame buffer onto the single sampled frame buffer using resolve.
	m_MultisampleFBO.Blit(m_ResolveFBO);
	m_MultisampleFBO.Unbind();

	renderer.Clear();
	m_ResolveFBO.GetColorTexture().Bind(); //Get texture of resolve fbo
	m_ScreenShader.SetUniform1f("u_Offset", m_KernelOffset);
	m_ScreenShader.SetUniform1fv("u_Kernel", 9, m_Kernel);
	glDisable(GL_DEPTH_TEST);
	renderer.DrawArray(m_VAO2, m_ScreenShader);


	m_Camera.UpdateCameraVectors();

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


	m_VAO = VertexArray();
	m_VAO.Bind();
	m_VBO = VertexBuffer(vertices, sizeof(vertices));
	m_VAO.RecordVBOLayout(m_VBO, layout);
	m_VAO.Unbind();

	m_VAO2 = VertexArray();
	m_VAO2.Bind();
	m_VBO2 = VertexBuffer(ndcQuadVertices, sizeof(ndcQuadVertices));
	m_VAO2.RecordVBOLayout(m_VBO2, layout2);
	m_VAO2.Unbind();

	m_StoneTexture = Texture("res/textures/MinecraftStone.jpg");
	m_StoneTexture.Bind(0);
	m_LampTexture = Texture("res/textures/MinecraftLamp.png");
	m_LampTexture.Bind(1);

	m_Shader = ShaderProgram("res/shaders/BlendingSceneShader.shader");
	m_ScreenShader = ShaderProgram("res/shaders/FramebufferShader.shader");

	m_Camera = Camera(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera.Fov = 80;
}
