#include "DepthTestingScene.h"
#include "vendor/imgui/imgui.h"


scene::DepthTestingScene::DepthTestingScene()
{
	DoPreviousInit();

}

scene::DepthTestingScene::~DepthTestingScene()
{
}

void scene::DepthTestingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::DepthTestingScene::OnRender()
{
	Renderer renderer;
	glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera.GetPerspectiveMatrix();

	glEnable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	m_StoneTexture.Bind(0);
	m_LampTexture.Bind(1);


	m_DepthShader.Bind();
	m_DepthShader.SetUniformMat4f("u_View", viewMatrix);
	m_DepthShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_DepthShader.SetUniform1f("u_Near", m_Camera.Near);
	m_DepthShader.SetUniform1f("u_Far", m_Camera.Far);

	//Stone floor
	glm::mat4 modelMatrixStone = glm::scale(glm::mat4(1), glm::vec3(4));
	m_DepthShader.SetUniformMat4f("u_Model", modelMatrixStone);
	m_DepthShader.SetUniform1i("u_Texture", 0);
	renderer.DrawArray(m_VAO, m_DepthShader);

	//Lamp 1
	glm::mat4 modelMatrixLamp1 = glm::translate(glm::mat4(1), glm::vec3(1, 2.5, -1));
	m_DepthShader.SetUniformMat4f("u_Model", modelMatrixLamp1);
	m_DepthShader.SetUniform1i("u_Texture", 1);
	renderer.DrawArray(m_VAO, m_DepthShader);

	//Lamp 2 - drawn with stencil test on
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	glm::mat4 modelMatrixLamp2 = glm::translate(glm::mat4(1), glm::vec3(-1, 2.5, 1));
	m_DepthShader.SetUniformMat4f("u_Model", modelMatrixLamp2);
	m_DepthShader.SetUniform1i("u_Texture", 1);
	renderer.DrawArray(m_VAO, m_DepthShader);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	m_StencilShader.Bind();
	glm::mat4 modelMatrixLampOutline = glm::scale(glm::translate(glm::mat4(1), glm::vec3(-1, 2.5, 1)), glm::vec3(1.05));
	m_StencilShader.SetUniformMat4f("u_View", viewMatrix);
	m_StencilShader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_StencilShader.SetUniformMat4f("u_Model", modelMatrixLampOutline);
	renderer.DrawArray(m_VAO, m_StencilShader);

	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);

	m_Camera.UpdateCameraVectors();
}

void scene::DepthTestingScene::OnImGuiRender()
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

void scene::DepthTestingScene::DoPreviousInit()
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

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);


	m_VAO = VertexArray();
	m_VAO.Bind();

	m_VBO = VertexBuffer(vertices, sizeof(vertices));
	m_VAO.RecordVBOLayout(m_VBO, layout);
	m_VAO.Unbind();

	m_StoneTexture = Texture("res/textures/MinecraftStone.jpg");
	m_StoneTexture.Bind(0);
	m_LampTexture = Texture("res/textures/MinecraftLamp.png");
	m_LampTexture.Bind(1);

	m_DepthShader = ShaderProgram("res/shaders/DepthTestingSceneShader.shader");
	m_StencilShader = ShaderProgram("res/shaders/StencilTestingSceneShader.shader");

	m_Camera = Camera(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera.Fov = 80;
	m_Camera.Far = 10;
}
