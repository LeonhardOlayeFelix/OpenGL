#include "AdvancedLightingScene.h"
#include "vendor/imgui/imgui.h"

scene::AdvancedLightingScene::AdvancedLightingScene()
{
	DoPreviousInit();
	m_DirectionalLight = DirectionalLight(glm::vec3(0.01), glm::vec3(0.5), glm::vec3(0.1), glm::vec3(1, -1, -1));
	m_PointLight = PointLight(glm::vec3(0.01), glm::vec3(0.5), glm::vec3(0.1), glm::vec3(0, 5, 0), 1, 0.09f, 0.1f);
	
	m_ShadowMap2DFramebuffer = FrameBuffer(2048, 2048);
	m_ShadowMap2DFramebuffer.AddAttachment(AttachmentTarget::Depth, AttachmentStorage::Texture);
	m_ShadowMap2DFramebuffer.MarkAsNoColorBuffer();

	m_ShadowMap2DShader = ShaderProgram("res/shaders/SimpleDepthShader.shader");
	m_ShadowMap2DShader.SetUniformBlockBinding("Matrices", 0);

	m_QuadShader = ShaderProgram("res/shaders/QuadShaderOrthographic.shader");

	m_ShadowMap3DFramebuffer = FrameBuffer(2048, 2048);
	m_ShadowMap3DFramebuffer.AddAttachment(AttachmentTarget::Depth, AttachmentStorage::CubeMap);
	m_ShadowMap3DFramebuffer.MarkAsNoColorBuffer();

	m_ShadowMap3DShader = ShaderProgram("res/shaders/SimpleDepthShader3D.shader");
	m_ShadowMap3DShader.SetUniformBlockBinding("Matrices", 0);

	m_CubeMapShader = ShaderProgram("res/shaders/QuadShader3D.shader");
}

scene::AdvancedLightingScene::~AdvancedLightingScene()
{
}

void scene::AdvancedLightingScene::OnUpdate(double deltaTime, GLFWwindow* window)
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

void scene::AdvancedLightingScene::OnRender()
{
	Renderer renderer;
	m_UBO.SetData(glm::value_ptr(m_Camera.GetViewMatrix()), sizeof(glm::mat4), sizeof(glm::mat4));
	m_UBO.SetData(glm::value_ptr(m_DirectionalLight.GetLightSpaceMatrix()), sizeof(glm::mat4), 2 * sizeof(glm::mat4));
	m_UBO.SetData(m_PointLight.GetLightSpaceMatrices().data(), 6 * sizeof(glm::mat4), 3 * sizeof(glm::mat4));

	glViewport(0, 0, 2048, 2048);
	m_ShadowMap2DFramebuffer.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT);
	m_ShadowMap2DShader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_ShadowMap2DShader);
	m_ShadowMap2DShader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_ShadowMap2DShader);
	m_ShadowMap2DShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_ShadowMap2DShader);
	const Texture& tex = m_ShadowMap2DFramebuffer.GetDepthTexture();
	GLCall(glTextureParameteri(tex.GetId(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTextureParameteri(tex.GetId(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLCall(glTextureParameterfv(tex.GetId(), GL_TEXTURE_BORDER_COLOR, borderColor));
	tex.Bind(2);
	glCullFace(GL_BACK);
	m_ShadowMap2DFramebuffer.Unbind();
	glViewport(0, 0, 1920, 1080);



	glViewport(0, 0, 2048, 2048);
	m_ShadowMap3DFramebuffer.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT); 
	m_ShadowMap3DShader.SetUniform1f("u_FarPlane", m_PointLight.Far);
	m_ShadowMap3DShader.SetUniform3f("u_LightPos", m_PointLight.Position);
	m_ShadowMap3DShader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_ShadowMap3DShader);
	m_ShadowMap3DShader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_ShadowMap3DShader);
	m_ShadowMap3DShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_ShadowMap3DShader);
	const CubeMap& cubeMap = m_ShadowMap3DFramebuffer.GetDepthCubeMap();
	cubeMap.Bind(3);
	glCullFace(GL_BACK);
	m_ShadowMap3DFramebuffer.Unbind();
	glViewport(0, 0, 1920, 1080);

	

	m_Shader.SetUniform1DirectionalLight("u_DirectionalLight", m_DirectionalLight);
	m_Shader.SetUniform1PointLight("u_PointLight", m_PointLight);
	m_Shader.SetUniform3f("u_ViewPos", m_Camera.Position);
	m_Shader.SetUniform1i("u_DirectionalLightShadowMap", 2);
	m_Shader.SetUniform1i("u_DepthTexture3D", 3);
	m_Shader.SetUniform3f("u_ViewPosition", m_Camera.Position);
	m_Shader.SetUniform1i("u_WoodMaterial.texture_diffuse1", 0);
	m_Shader.SetUniform1i("u_WoodMaterial.texture_specular1", 1);
	m_Shader.SetUniform1f("u_WoodMaterial.shininess", m_Shininess);
	m_Shader.SetUniform1f("u_WoodMaterial.blinn", m_IsBlinn);
	m_Shader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_Shader);
	m_Shader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_Shader);
	m_Shader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_Shader);
	m_WhiteCubeShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_PointLight.Position), glm::vec3(0.5)));
	renderer.DrawArray(m_VAO, m_WhiteCubeShader);

	float aspect = 1920.0f / 1080.0f;

	m_QuadShader.SetUniform1i("u_DepthTexture", 2);
	m_QuadShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(1.35, 0.6, 0)), glm::vec3(0.375)));
	m_QuadShader.SetUniformMat4f("u_Proj", glm::ortho(-aspect, aspect, -1.0f, 1.0f));
	renderer.DrawArray(m_VAO2, m_QuadShader);

	m_CubeMapShader.SetUniform1i("u_DepthTexture3D", 3);
	m_CubeMapShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_CubeMapCenter), glm::vec3(3)));
	m_CubeMapShader.SetUniformMat4f("u_Proj", m_Camera.GetPerspectiveMatrix());
	m_CubeMapShader.SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
	renderer.DrawArray(m_VAO, m_CubeMapShader);

	m_Camera.UpdateCameraVectors();
}

void scene::AdvancedLightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Material Settings"))
	{
		ImGui::SliderFloat("Shininess", &m_Shininess, 1, 512);
		ImGui::Checkbox("Use blinn", &m_IsBlinn);
	}
	if (ImGui::CollapsingHeader("Light Settings"))
	{
		ImGui::SliderFloat3("Direcitonal Light Direction", glm::value_ptr(m_DirectionalLight.Direction), -1, 1);
		ImGui::SliderFloat3("Point Light Position", glm::value_ptr(m_PointLight.Position), -10, 10);
		ImGui::SliderFloat3("CubeMap Center Position", glm::value_ptr(m_CubeMapCenter), -10, 10);
		if (ImGui::Button("View point light depth cube map")) {
			m_Camera.Position = m_CubeMapCenter;
		}
	}


	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::AdvancedLightingScene::DoPreviousInit()
{
	m_Camera = Camera(glm::vec3(-6.35, 5.2, -8.1), glm::vec3(0, 1, 0), 60, -26.0f);

	m_UBO = UniformBuffer(9 * sizeof(glm::mat4));
	m_UBO.BindToPoint(0);
	m_UBO.SetData(glm::value_ptr(m_Camera.GetPerspectiveMatrix()), sizeof(glm::mat4), 0);

	m_VBO = VertexBuffer(Primitives::CubePNT());
	VertexBufferLayout layout{ 3, 3, 2 };
	m_VAO.RecordVBOLayout(m_VBO, layout);

	m_VBO2 = VertexBuffer(Primitives::NDCQuad());
	VertexBufferLayout layout2{ 3, 2 };
	m_VAO2.RecordVBOLayout(m_VBO2, layout2);

	m_Shader = ShaderProgram("res/shaders/BlinnPhongLightingShader.shader");
	m_Shader.SetUniformBlockBinding("Matrices", 0);
	m_WhiteCubeShader = ShaderProgram("res/shaders/WhiteCubeShader.shader");
	m_WhiteCubeShader.SetUniformBlockBinding("Matrices", 0);

	m_WoodDiffuse = Texture("res/textures/WoodTiles.jpg", GL_SRGB8_ALPHA8);
	m_WoodDiffuse.Bind(0);
	m_WoodSpecular = Texture("res/textures/White.jpg");
	m_WoodSpecular.Bind(1);
}
