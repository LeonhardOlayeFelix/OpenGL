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
		 
		VertexArray   m_VAO;
		VertexArray   m_VAO2;
		VertexBuffer  m_VBO;
		VertexBuffer  m_VBO2;
		UniformBuffer m_UBO;
		ShaderProgram m_SceneShader;
		ShaderProgram m_QuadShader;
		ShaderProgram m_WhiteCubeShader;
		ShaderProgram m_CubeShader;
		ShaderProgram m_DepthMapShader1;
		ShaderProgram m_DepthMapShader2;
		FrameBuffer   m_FrameBuffer1;
		FrameBuffer   m_FrameBuffer2;
		FrameBuffer   m_FrameBuffer3;
		Camera        m_Camera;
		Texture		  m_WoodDiffuse;
		Texture	      m_WoodSpecular;
		PointLight					   m_PointLight{};
		DirectionalLight			   m_DirectionalLight{};
		SpotLight					   m_SpotLight{};
		float						   m_Shininess = 256;
		bool						   m_IsBlinn = true;
		glm::vec3					   m_CubeMapCenter = glm::vec3(-8.0, 0.0, 0.0);

	private:
		void DoPreviousInit();
	};
}