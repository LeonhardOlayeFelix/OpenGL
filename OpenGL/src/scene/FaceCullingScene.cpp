#include "FaceCullingScene.h"
#include "vendor/imgui/imgui.h"


scene::FaceCullingScene::FaceCullingScene()
{
	DoPreviousInit();

	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		 // Bottom face
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 // Top face
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	m_VAO = VertexArray();
	m_VBO = VertexBuffer(cubeVertices, sizeof(cubeVertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO.RecordVBOLayout(m_VBO, layout);

	m_Shader = ShaderProgram("res/shaders/FaceCullingSceneShader.shader");
	m_Texture = Texture("res/textures/MinecraftLamp.png");
}

scene::FaceCullingScene::~FaceCullingScene()
{
}

void scene::FaceCullingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::FaceCullingScene::OnRender()
{
	Renderer renderer;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
	glm::mat4 perspectiveMatrix = m_Camera.GetPerspectiveMatrix();
	glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(4));

	m_Texture.Bind(0);

	m_Shader.Bind();
	m_Shader.SetUniformMat4f("u_View", viewMatrix);
	m_Shader.SetUniformMat4f("u_Proj", perspectiveMatrix);
	m_Shader.SetUniformMat4f("u_Model", modelMatrix);
	m_Shader.SetUniform1i("u_Texture", 0);

	renderer.DrawArray(m_VAO, m_Shader);

	m_Camera.UpdateCameraVectors();
}

void scene::FaceCullingScene::OnImGuiRender()
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

void scene::FaceCullingScene::DoPreviousInit()
{
	m_Camera = Camera(glm::vec3(2, 3, 2), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera.Fov = 80;
}
