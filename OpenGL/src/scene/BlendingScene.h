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

	class BlendingScene : public Scene
	{
	public:
		BlendingScene();
		~BlendingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;
		 
		VertexArray   m_VAO;
		VertexArray   m_VAO2;
		VertexBuffer  m_VBO;
		VertexBuffer  m_VBO2;
		IndexBuffer   m_IBO;
		ShaderProgram m_BlockShader;
		Texture       m_StoneTexture;
		Texture       m_LampTexture;
		Texture       m_GrassTexture;
		Texture       m_WindowTexture;
		Camera        m_Camera;
		std::vector<glm::vec3>		   m_Vegetation;
		std::vector<glm::vec3>		   m_Windows;

	private:
		void DoPreviousInit();
	};
}