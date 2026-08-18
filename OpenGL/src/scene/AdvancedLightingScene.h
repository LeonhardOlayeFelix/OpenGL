#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include "Model.h"

namespace scene {

	class AdvancedLightingScene : public Scene
	{
	public:
		AdvancedLightingScene();
		~AdvancedLightingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;
		 
		Camera        m_Camera;
		VertexArray   m_VAO;
		VertexArray   m_VAO2;
		VertexArray   m_VAO3;
		VertexBuffer  m_VBO;
		VertexBuffer  m_VBO2;
		VertexBuffer  m_VBO3;
		IndexBuffer   m_IBO;
		UniformBuffer m_UBO;
		UniformBuffer m_UBO2;
		ShaderProgram m_SceneShader;
		ShaderProgram m_LampShader;
		ShaderProgram m_QuadShader;
		ShaderProgram m_CubeShader;
		ShaderProgram m_DepthMapShader1;
		ShaderProgram m_DepthMapShader2;
		FrameBuffer   m_FrameBuffer1;
		FrameBuffer   m_FrameBuffer2;
		FrameBuffer   m_FrameBuffer3;
		Material	  m_Material1;
		Material	  m_Material2;
		Material	  m_Material3;
		PointLight		 m_PointLight{};
		DirectionalLight m_DirectionalLight{};
		SpotLight		 m_SpotLight{};
		float			 m_Shininess = 256;
		bool			 m_IsBlinn = true;


		//ImGui constants
		glm::vec3 m_CubeMapCenter = glm::vec3(-8.0, 0.0, 0.0);
		bool m_DepthMapVisible = false;

	private:
		void DoPreviousInit();
		void RenderDepthMaps();
		void DisplayDepthMaps();
		void RenderLightedScene();
		void RenderLights();
	};
}