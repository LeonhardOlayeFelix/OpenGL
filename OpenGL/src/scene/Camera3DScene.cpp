#include "Camera3DScene.h"
#include "vendor/imgui/imgui.h"

scene::Camera3DScene::Camera3DScene()
{

	m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);


	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	m_CubePositions = {
	glm::vec3(0.0f,  4.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	m_VAO = std::make_unique<VertexArray>();
	m_VAO->Bind();

	m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	m_VAO->RecordVBOLayout(*m_VBO, layout);

	m_Shader = std::make_unique<ShaderProgram>("res/shaders/Transformations3DSceneShader.shader");
	m_Shader->Bind();
	m_Shader->SetUniform1i("u_Texture", 0);
	m_Shader->SetUniform1i("u_Texture2", 1);

	m_Texture = std::make_unique<Texture>("res/textures/container.jpg");
	m_Texture->Bind(0);

	m_Texture2 = std::make_unique<Texture>("res/textures/awesomeface.png");
	m_Texture2->Bind(1);






	

}

scene::Camera3DScene::~Camera3DScene()
{
}

void scene::Camera3DScene::OnUpdate(float deltaTime, GLFWwindow* window)
{
	glm::vec3 cameraFront = GetCameraFront();
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_CameraPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_CameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, m_CameraUp));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, m_CameraUp));
}

void scene::Camera3DScene::OnRender()
{

}

void scene::Camera3DScene::OnImGuiRender()
{
	Renderer renderer;
	m_Shader->Bind();
	m_Texture->Bind(0);
	m_Texture2->Bind(1);

	ImGui::Begin("Object Transformations");
	ImGui::SliderFloat("Translation X", &m_TranslateX, -2.0f, 2.0f);
	ImGui::SliderFloat("Translation Y", &m_TranslateY, -2.0f, 2.0f);
	ImGui::SliderFloat("Translation Z", &m_TranslateZ, -2.0f, 6.0f);
	ImGui::SliderAngle("Angle", &m_Angle, 0.0f, 360.0f);
	ImGui::SliderFloat("Scale X", &m_ScaleX, 0.0f, 2.0f);
	ImGui::SliderFloat("Scale Y", &m_ScaleY, 0.0f, 2.0f);
	ImGui::SliderFloat("Scale Z", &m_ScaleZ, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Camera Transformation");
	ImGui::SliderFloat("Fov", &m_Fov, 0.0f, 360.0f);
	ImGui::SliderFloat("Yaw", &m_Yaw, -180.0f, 180.0f);
	ImGui::SliderFloat("Pitch", &m_Pitch, -180.0f, 180.0f);
	ImGui::End();

	m_ModelTransform = glm::mat4(1.0f);
	m_ModelTransform = glm::translate(m_ModelTransform, glm::vec3(m_TranslateX, m_TranslateY, m_TranslateZ));
	m_ModelTransform = glm::rotate(m_ModelTransform, glm::degrees(m_Angle / 180.0f * 3.14f), glm::vec3(1.0, 1.0, -1.0));
	m_ModelTransform = glm::scale(m_ModelTransform, glm::vec3(m_ScaleX, m_ScaleY, m_ScaleZ));
	m_Shader->SetUniformMat4f("u_Model", m_ModelTransform);


	m_ViewTransform = glm::lookAt(m_CameraPosition, m_CameraPosition + GetCameraFront() , m_CameraUp);
	m_Shader->SetUniformMat4f("u_View", m_ViewTransform);


	m_ProjTransform = glm::perspective(glm::radians(m_Fov), 800.0f / 600.0f, 0.1f, 100.0f);
	m_Shader->SetUniformMat4f("u_Proj", m_ProjTransform);


	renderer.DrawArray(*m_VAO, *m_Shader);

	for (unsigned int i = 0; i < 10; i++){
		m_ModelTransform = glm::mat4(1.0f);
		m_ModelTransform = glm::translate(m_ModelTransform, m_CubePositions[i]);
		m_ModelTransform = glm::rotate(m_ModelTransform, glm::degrees(m_Angle / 180.0f * 3.14f), glm::vec3(1.0, 1.0, -1.0));
		m_Shader->SetUniformMat4f("u_Model", m_ModelTransform);
		renderer.DrawArray(*m_VAO, *m_Shader);
	}



}


