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

	class CubeMapScene : public Scene
	{
	public:
		CubeMapScene();
		~CubeMapScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;
		 
		VertexArray m_VAO{};
		VertexArray m_VAO2{};
		VertexBuffer m_VBO{};
		VertexBuffer m_VBO2{};
		UniformBuffer m_UBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
		ShaderProgram m_ReflectionShader{};
		ShaderProgram m_RefractionShader{};
		ShaderProgram m_SkyboxShader{};
		Model m_Model{};
		Camera m_Camera{};
		CubeMap m_CubeMap{};
		Texture m_Texture{};

	private:
		void DoPreviousInit();
	};
}
