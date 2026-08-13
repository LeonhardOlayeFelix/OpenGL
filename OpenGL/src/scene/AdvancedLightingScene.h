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
		 
		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<UniformBuffer> m_UBO;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<Camera>        m_Camera;
		std::unique_ptr<Texture>	   m_WoodDiffuse;
		std::unique_ptr<Texture>	   m_WoodSpecular;

	private:
		void DoPreviousInit();
	};
}