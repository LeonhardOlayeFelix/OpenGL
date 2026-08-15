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
		ShaderProgram m_Shader;
		ShaderProgram m_QuadShader;
		ShaderProgram m_DepthShader;
		Camera        m_Camera;
		Texture		  m_WoodDiffuse;
		Texture	      m_WoodSpecular;
		FrameBuffer   m_ShadowFramebuffer;
		PointLight					   m_PointLight{};
		DirectionalLight			   m_DirectionalLight{};
		SpotLight					   m_SpotLight{};
		float						   m_Shininess = 256;
		bool						   m_IsBlinn = true;

	private:
		void DoPreviousInit();
	};
}