#include "CubeMapScene.h"
#include "vendor/imgui/imgui.h"


scene::CubeMapScene::CubeMapScene()
{
	DoPreviousInit();

}

scene::CubeMapScene::~CubeMapScene()
{
}

void scene::CubeMapScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::CubeMapScene::OnRender()
{
	Renderer renderer;

	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();
	m_UBO->SetData(glm::value_ptr(viewMatrix), sizeof(glm::mat4), sizeof(glm::mat4));

	

	m_Texture->Bind(1);
	m_ReflectionShader->SetUniform1i("u_Texture", 1);
	m_ReflectionShader->SetUniformMat4f("u_Model", glm::translate(glm::mat4(1.0), glm::vec3(2, 0, 0)));
	renderer.DrawArray(*m_VAO, *m_ReflectionShader);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	m_CubeMap->Bind(0);
	m_SkyboxShader->SetUniform1i("u_Skybox", 0);
	m_SkyboxShader->SetUniformMat4f("u_ViewNoTranslation", glm::mat4(glm::mat3(viewMatrix)));
	renderer.DrawArray(*m_VAO2, *m_SkyboxShader);
	glDepthMask(GL_TRUE);

	m_Camera->UpdateCameraVectors();
}

void scene::CubeMapScene::OnImGuiRender()
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

void scene::CubeMapScene::DoPreviousInit()
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
	float skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
	};
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);
	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_VAO2 = std::make_unique<VertexArray>();
	m_VBO2 = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
	VertexBufferLayout layout2;
	layout2.Push<float>(3);
	m_VAO2->RecordVBOLayout(*m_VBO2, layout2);

	m_Camera = std::make_unique<Camera>(glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, -20.0f);
	m_Camera->Fov = 80;

	//m_Shader = std::make_unique<ShaderProgram>("res/shaders/CubeMapSceneShader1.shader");
	//m_Shader->SetUniformBlockBinding("Matrices", 0);

	m_SkyboxShader = std::make_unique<ShaderProgram>("res/shaders/CubeMapSceneSkyboxShader.shader");
	m_SkyboxShader->SetUniformBlockBinding("Matrices", 0);

	m_ReflectionShader = std::make_unique<ShaderProgram>("res/shaders/CubeMapSceneReflectionShader.shader");
	m_ReflectionShader->SetUniformBlockBinding("Matrices", 0);




	std::array<std::string, 6> paths;
	paths[RIGHT] = "res/textures/skybox/right.jpg";
	paths[LEFT] = "res/textures/skybox/left.jpg";
	paths[FRONT] = "res/textures/skybox/front.jpg";
	paths[BACK] = "res/textures/skybox/back.jpg";
	paths[TOP] = "res/textures/skybox/top.jpg";
	paths[BOTTOM] = "res/textures/skybox/bottom.jpg";

	m_CubeMap = std::make_unique<CubeMap>(paths);
	m_CubeMap->Bind(0);
	
	m_Texture = std::make_unique<Texture>("res/textures/skybox/front.jpg");
	m_Texture->Bind(1);

	m_UBO = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4));
	m_UBO->BindToPoint(0);

	glm::mat4 perspectiveMatrix = m_Camera->GetPerspectiveMatrix();
	m_UBO->SetData(glm::value_ptr(perspectiveMatrix), sizeof(glm::mat4), 0);
}
