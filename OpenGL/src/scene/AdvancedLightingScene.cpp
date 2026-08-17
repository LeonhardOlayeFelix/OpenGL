#include "AdvancedLightingScene.h"
#include "vendor/imgui/imgui.h"
#include "lights/LightGuiDrawer.h"

scene::AdvancedLightingScene::AdvancedLightingScene()
{
	DoPreviousInit();
	m_DirectionalLight = DirectionalLight(glm::vec3(0.01), glm::vec3(0.5), glm::vec3(0.1), glm::vec3(1, -1, -1));
	m_PointLight = PointLight(glm::vec3(0.01), glm::vec3(0.5), glm::vec3(0.1), glm::vec3(0, 5, 0), 1, 0.09f, 0.1f);
	m_PointLight.Far = 10;
	m_SpotLight = SpotLight(glm::vec3(0.01), glm::vec3(0.5), glm::vec3(0.1), glm::vec3(0, 5, 0), glm::vec3(0, -1, 0), 1, 0.09f, 0.1f, 12.5, 17.5);

	m_FrameBuffer1 = FrameBuffer(2048, 2048);
	m_FrameBuffer1.AddAttachment(AttachmentTarget::Depth, AttachmentStorage::Texture);
	m_FrameBuffer1.MarkAsNoColorBuffer();

	m_FrameBuffer2 = FrameBuffer(2048, 2048);
	m_FrameBuffer2.AddAttachment(AttachmentTarget::Depth, AttachmentStorage::Texture);
	m_FrameBuffer2.MarkAsNoColorBuffer();

	m_FrameBuffer3 = FrameBuffer(2048, 2048);
	m_FrameBuffer3.AddAttachment(AttachmentTarget::Depth, AttachmentStorage::CubeMap);
	m_FrameBuffer3.MarkAsNoColorBuffer();

	m_DepthMapShader1 = ShaderProgram("res/shaders/SimpleDepthShader.shader");

	m_DepthMapShader2 = ShaderProgram("res/shaders/SimpleDepthShader3D.shader");
	m_DepthMapShader2.SetUniformBlockBinding("Matrices", 0);

	m_QuadShader = ShaderProgram("res/shaders/QuadShaderPerspective.shader");


	m_CubeShader = ShaderProgram("res/shaders/QuadShader3D.shader");
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
	float aspect = 1920.0f / 1080.0f;

	Renderer renderer;
	m_UBO.SetData(glm::value_ptr(m_Camera.GetViewMatrix()), sizeof(glm::mat4), sizeof(glm::mat4));
	m_UBO.SetData(glm::value_ptr(m_DirectionalLight.GetLightSpaceMatrix()), sizeof(glm::mat4), 2 * sizeof(glm::mat4));
	m_UBO.SetData(m_PointLight.GetLightSpaceMatrices().data(), 6 * sizeof(glm::mat4), 3 * sizeof(glm::mat4));
	m_UBO.SetData(glm::value_ptr(m_SpotLight.GetLightSpaceMatrix()), sizeof(glm::mat4), 9 * sizeof(glm::mat4));

	//Directional light
	glViewport(0, 0, 2048, 2048);
	m_FrameBuffer1.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT);
	m_DepthMapShader1.SetUniformMat4f("u_View", m_DirectionalLight.GetLightSpaceMatrix());
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_FrameBuffer1.GetDepthTexture().Bind(2);
	glCullFace(GL_BACK);
	m_FrameBuffer1.Unbind();
	glViewport(0, 0, 1920, 1080);

	//spot light
	glViewport(0, 0, 2048, 2048);
	m_FrameBuffer2.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT);
	m_DepthMapShader1.SetUniformMat4f("u_View", m_SpotLight.GetLightSpaceMatrix());
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_DepthMapShader1.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader1);
	m_FrameBuffer2.GetDepthTexture().Bind(3);
	glCullFace(GL_BACK);
	m_FrameBuffer2.Unbind();
	glViewport(0, 0, 1920, 1080);


	//point light
	glViewport(0, 0, 2048, 2048);
	m_FrameBuffer3.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT); 
	m_DepthMapShader2.SetUniform1f("u_FarPlane", 10);
	m_DepthMapShader2.SetUniform3f("u_LightPos", m_PointLight.Position);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_FrameBuffer3.GetDepthCubeMap().Bind(4);
	glCullFace(GL_BACK);
	m_FrameBuffer3.Unbind();
	glViewport(0, 0, 1920, 1080);

	

	m_SceneShader.SetUniform1DirectionalLight("u_DirectionalLight", m_DirectionalLight);
	m_SceneShader.SetUniform1PointLight("u_PointLight", m_PointLight);
	m_SceneShader.SetUniform1SpotLight("u_SpotLight", m_SpotLight);
	m_SceneShader.SetUniform3f("u_ViewPosition", m_Camera.Position);
	m_SceneShader.SetUniform1i("u_WoodMaterial.texture_diffuse1", 0);
	m_SceneShader.SetUniform1i("u_WoodMaterial.texture_specular1", 1);
	m_SceneShader.SetUniform1i("u_DepthTexture1", 2);
	m_SceneShader.SetUniform1i("u_DepthTexture2", 3);
	m_SceneShader.SetUniform1i("u_DepthTexture3D", 4);
	m_SceneShader.SetUniform1f("u_WoodMaterial.shininess", m_Shininess);
	m_SceneShader.SetUniform1f("u_WoodMaterial.blinn", m_IsBlinn);
	m_SceneShader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_SceneShader);
	m_SceneShader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_SceneShader);
	m_SceneShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_SceneShader);
	m_WhiteCubeShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_SpotLight.Position), glm::vec3(0.5)));
	renderer.DrawArray(m_VAO, m_WhiteCubeShader);

	//Quad drawing
	m_QuadShader.SetUniform1i("u_DepthTexture", 2);
	m_QuadShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(1.5, 0.7, 0)), glm::vec3(0.2)));
	m_QuadShader.SetUniformMat4f("u_Proj", glm::ortho(-aspect, aspect, -1.0f, 1.0f));
	m_QuadShader.SetUniform1i("u_Linearize", 0);
	renderer.DrawArray(m_VAO2, m_QuadShader);
	
	//Quad drawing
	m_QuadShader.SetUniform1i("u_DepthTexture", 3);
	m_QuadShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(1.5, -0.6, 0)), glm::vec3(0.2)));
	m_QuadShader.SetUniform1i("u_Linearize", 1);
	m_QuadShader.SetUniform1f("u_NearPlane", 0.1);
	m_QuadShader.SetUniform1f("u_FarPlane", 10);
	renderer.DrawArray(m_VAO2, m_QuadShader);

	//Cube map drawing
	m_CubeShader.SetUniform1i("u_DepthTexture3D", 4);
	m_CubeShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_CubeMapCenter), glm::vec3(3)));
	m_CubeShader.SetUniformMat4f("u_Proj", m_Camera.GetPerspectiveMatrix());
	m_CubeShader.SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
	renderer.DrawArray(m_VAO, m_CubeShader);

	m_Camera.UpdateCameraVectors();
}

void scene::AdvancedLightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	LightGuiDrawer::DrawObjectInspector(m_DirectionalLight, "Directional Light");
	LightGuiDrawer::DrawObjectInspector(m_PointLight, "Point Light");
	LightGuiDrawer::DrawObjectInspector(m_SpotLight, "Spot Light");

	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void scene::AdvancedLightingScene::DoPreviousInit()
{
	m_Camera = Camera(glm::vec3(-6.35, 5.2, -8.1), glm::vec3(0, 1, 0), 60, -26.0f);

	m_UBO = UniformBuffer(10 * sizeof(glm::mat4));
	m_UBO.BindToPoint(0);
	m_UBO.SetData(glm::value_ptr(m_Camera.GetPerspectiveMatrix()), sizeof(glm::mat4), 0);

	m_VBO = VertexBuffer(Primitives::CubePNT());
	VertexBufferLayout layout{ 3, 3, 2 };
	m_VAO.RecordVBOLayout(m_VBO, layout);

	m_VBO2 = VertexBuffer(Primitives::NDCQuad());
	VertexBufferLayout layout2{ 3, 2 };
	m_VAO2.RecordVBOLayout(m_VBO2, layout2);

	m_SceneShader = ShaderProgram("res/shaders/BlinnPhongLightingShader.shader");
	m_SceneShader.SetUniformBlockBinding("Matrices", 0);
	m_WhiteCubeShader = ShaderProgram("res/shaders/WhiteCubeShader.shader");
	m_WhiteCubeShader.SetUniformBlockBinding("Matrices", 0);

	m_WoodDiffuse = Texture("res/textures/WoodTiles.jpg", GL_SRGB8_ALPHA8);
	m_WoodDiffuse.Bind(0);
	m_WoodSpecular = Texture("res/textures/White.jpg");
	m_WoodSpecular.Bind(1);
}
