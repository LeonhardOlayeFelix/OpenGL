#include "AdvancedLightingScene.h"
#include "vendor/imgui/imgui.h"
#include "lights/LightGuiDrawer.h"

scene::AdvancedLightingScene::AdvancedLightingScene()
{
	DoPreviousInit();
	m_DirectionalLight = DirectionalLight(glm::vec3(0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0), glm::vec3(0.428, 0.714, 10.0));
	m_PointLight = PointLight(glm::vec3(0.03, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.1), glm::vec3(-2.353, 2.353, 1.765), 1, 0.09f, 0.1f);
	m_SpotLight = SpotLight(glm::vec3(0.0, 0.03, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.1), glm::vec3(4.412, 3.551, -2.029), glm::vec3(-1.471, -1.0, 0.0), 1, 0.09f, 0.1f, 15, 21);

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
	m_DepthMapShader2.SetUniformBlockBinding("CameraMatrices", 0);
	m_DepthMapShader2.SetUniformBlockBinding("LightMatrices", 1);

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
	m_UBO2.SetData(glm::value_ptr(m_DirectionalLight.GetLightSpaceMatrix()), sizeof(glm::mat4), 0);
	m_UBO2.SetData(m_PointLight.GetLightSpaceMatrices().data(), 6 * sizeof(glm::mat4), sizeof(glm::mat4));
	m_UBO2.SetData(glm::value_ptr(m_SpotLight.GetLightSpaceMatrix()), sizeof(glm::mat4), 7 * sizeof(glm::mat4));


	RenderDepthMaps();

	RenderLights();

	RenderLightedScene();
	
	if (m_DepthMapVisible) 
		DisplayDepthMaps();
	

	m_Camera.UpdateCameraVectors();
}

void scene::AdvancedLightingScene::OnImGuiRender()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));
	ImGuiIO& io = ImGui::GetIO();

	static bool showLightSettings = false;

	if (ImGui::CollapsingHeader("Depth Map Settings")) {
		ImGui::Checkbox("Show Depth Maps", &m_DepthMapVisible);
		if (ImGui::Button("Teleport to CubeMap"))
			m_Camera.Position = m_CubeMapCenter;
	}

	if (ImGui::CollapsingHeader("Light Settings"))
	{
		LightGuiDrawer::DrawObjectInspector(m_DirectionalLight, "Directional Light");
		LightGuiDrawer::DrawObjectInspector(m_PointLight, "Point Light");
		LightGuiDrawer::DrawObjectInspector(m_SpotLight, "Spot Light");
	}



	ImGui::Separator();
	ImGui::TextDisabled("Move camera: WASD");
	ImGui::TextDisabled("Look around: JILK");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}



void scene::AdvancedLightingScene::RenderDepthMaps()
{
	Renderer renderer;
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
	m_FrameBuffer1.GetDepthTexture().Bind(0);
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
	m_FrameBuffer2.GetDepthTexture().Bind(1);
	glCullFace(GL_BACK);
	m_FrameBuffer2.Unbind();
	glViewport(0, 0, 1920, 1080);


	//point light
	glViewport(0, 0, 2048, 2048);
	m_FrameBuffer3.Bind();
	renderer.Clear();
	glCullFace(GL_FRONT);
	m_DepthMapShader2.SetUniform1f("u_FarPlane", m_PointLight.Far);
	m_DepthMapShader2.SetUniform3f("u_LightPos", m_PointLight.Position);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_DepthMapShader2.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_DepthMapShader2);
	m_FrameBuffer3.GetDepthCubeMap().Bind(2);
	glCullFace(GL_BACK);
	m_FrameBuffer3.Unbind();
	glViewport(0, 0, 1920, 1080);
}

void scene::AdvancedLightingScene::RenderLightedScene()
{
	Renderer renderer;
	//Depth Maps
	m_SceneShader.SetUniform1i("u_DepthTexture1", m_FrameBuffer1.GetDepthTexture().GetBoundSlot());
	m_SceneShader.SetUniform1i("u_DepthTexture2", m_FrameBuffer2.GetDepthTexture().GetBoundSlot());
	m_SceneShader.SetUniform1i("u_DepthTexture3D", m_FrameBuffer3.GetDepthCubeMap().GetBoundSlot());

	m_SceneShader.SetUniform3f("u_ViewPosition", m_Camera.Position);

	m_SceneShader.SetUniform1DirectionalLight("u_DirectionalLight", m_DirectionalLight);
	m_SceneShader.SetUniform1PointLight("u_PointLight", m_PointLight);
	m_SceneShader.SetUniform1SpotLight("u_SpotLight", m_SpotLight);

	//Floor
	m_SceneShader.SetUniformMat4f("u_Model", glm::scale(glm::mat4(1), glm::vec3(10, 1, 10)));
	m_SceneShader.SetUniform1Material("u_Material", m_Material1);
	renderer.DrawArray(m_VAO, m_SceneShader);

	//Boxes
	m_SceneShader.SetUniform1Material("u_Material", m_Material2);
	m_SceneShader.SetUniformMat4f("u_Model", glm::translate(glm::mat4(1), glm::vec3(0, 1, -2)));
	renderer.DrawArray(m_VAO, m_SceneShader);
	m_SceneShader.SetUniform1Material("u_Material", m_Material3);
	m_SceneShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 1.5, 2)), glm::vec3(2)));
	renderer.DrawArray(m_VAO, m_SceneShader);
}

void scene::AdvancedLightingScene::RenderLights()
{
	Renderer renderer;
	m_LampShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_SpotLight.Position), glm::vec3(0.5)));
	m_LampShader.SetUniform3f("u_Color", m_SpotLight.Diffuse);
	renderer.DrawElements(m_VAO3, m_LampShader);
	m_LampShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_PointLight.Position), glm::vec3(0.5)));
	m_LampShader.SetUniform3f("u_Color", m_PointLight.Diffuse);
	renderer.DrawElements(m_VAO3, m_LampShader);
}

void scene::AdvancedLightingScene::DisplayDepthMaps()
{
	float aspect = 1920.0f / 1080.0f;
	Renderer renderer;
	//Quad drawing
	m_QuadShader.SetUniform1i("u_DepthTexture", m_FrameBuffer1.GetDepthTexture().GetBoundSlot());
	m_QuadShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(1.5, 0.7, 0)), glm::vec3(0.2)));
	m_QuadShader.SetUniformMat4f("u_Proj", glm::ortho(-aspect, aspect, -1.0f, 1.0f));
	m_QuadShader.SetUniform1i("u_Linearize", 0);
	renderer.DrawArray(m_VAO2, m_QuadShader);

	//Quad drawing
	m_QuadShader.SetUniform1i("u_DepthTexture", m_FrameBuffer2.GetDepthTexture().GetBoundSlot());
	m_QuadShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), glm::vec3(1.5, -0.6, 0)), glm::vec3(0.2)));
	m_QuadShader.SetUniform1i("u_Linearize", 1);
	m_QuadShader.SetUniform1f("u_NearPlane", 0.1);
	m_QuadShader.SetUniform1f("u_FarPlane", 10);
	renderer.DrawArray(m_VAO2, m_QuadShader);

	//Cube map drawing
	m_CubeShader.SetUniform1i("u_DepthTexture3D", m_FrameBuffer3.GetDepthCubeMap().GetBoundSlot());
	m_CubeShader.SetUniformMat4f("u_Model", glm::scale(glm::translate(glm::mat4(1), m_CubeMapCenter), glm::vec3(3)));
	m_CubeShader.SetUniformMat4f("u_Proj", m_Camera.GetPerspectiveMatrix());
	m_CubeShader.SetUniformMat4f("u_View", m_Camera.GetViewMatrix());
	renderer.DrawArray(m_VAO, m_CubeShader);
}

void scene::AdvancedLightingScene::DoPreviousInit()
{
	m_Camera = Camera(glm::vec3(-6.35, 5.2, -8.1), glm::vec3(0, 1, 0), 60, -26.0f);

	m_UBO = UniformBuffer(2 * sizeof(glm::mat4));
	m_UBO.BindToPoint(0);
	m_UBO.SetData(glm::value_ptr(m_Camera.GetPerspectiveMatrix()), sizeof(glm::mat4), 0);

	m_UBO2 = UniformBuffer(8 * sizeof(glm::mat4));
	m_UBO2.BindToPoint(1);

	m_VBO = VertexBuffer(Primitives::CubePNT());
	m_VAO.RecordVBOLayout(m_VBO, VertexBufferLayout{ 3, 3, 2 });

	m_VBO2 = VertexBuffer(Primitives::NDCQuad());
	m_VAO2.RecordVBOLayout(m_VBO2, VertexBufferLayout{ 3, 2 });

	m_VBO3 = VertexBuffer(Primitives::SpherePNT());
	m_IBO = IndexBuffer(Primitives::SphereIndicies());
	m_VAO3.RecordVBOLayout(m_VBO3, VertexBufferLayout{ 3, 3, 2 });
	m_VAO3.RecordIndexBuffer(m_IBO);

	m_SceneShader = ShaderProgram("res/shaders/BlinnPhongLightingShader.shader");
	m_SceneShader.SetUniformBlockBinding("CameraMatrices", 0);
	m_SceneShader.SetUniformBlockBinding("LightMatrices", 1);

	m_LampShader = ShaderProgram("res/shaders/WhiteCubeShader.shader");
	m_LampShader.SetUniformBlockBinding("CameraMatrices", 0);
	m_LampShader.SetUniformBlockBinding("LightMatrices", 1);

	m_Material1 = Material("res/textures/brickwall.jpg", "res/textures/White.jpg", "res/textures/brickwall_normal.jpg");
	m_Material1.SetBindingConfig(MapBindingConfig(3, 4, 5));

	m_Material2 = Material("res/textures/WoodTiles.jpg", "res/textures/White.jpg", "res/textures/brickwall_normal.jpg");
	m_Material2.SetBindingConfig(MapBindingConfig(3, 4, 5));

	m_Material3 = Material("res/textures/MinecraftLamp.png", "res/textures/White.jpg", "res/textures/brickwall_normal.jpg");
	m_Material3.SetBindingConfig(MapBindingConfig(3, 4, 5));
}