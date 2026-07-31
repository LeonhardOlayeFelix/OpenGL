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
		 
		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexArray>   m_VAO2;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<VertexBuffer>  m_VBO2;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_BlockShader;
		std::unique_ptr<Texture>       m_StoneTexture;
		std::unique_ptr<Texture>       m_LampTexture;
		std::unique_ptr<Texture>       m_GrassTexture;
		std::unique_ptr<Texture>       m_WindowTexture;
		std::unique_ptr<Camera>        m_Camera;
		std::vector<glm::vec3>		   m_Vegetation;
		std::vector<glm::vec3>		   m_Windows;

	private:
		void DoPreviousInit();
	};
}