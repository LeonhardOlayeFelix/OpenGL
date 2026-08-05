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
		 
		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexArray>   m_VAO2;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<VertexBuffer>  m_VBO2;
		std::unique_ptr<UniformBuffer> m_UBO;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<ShaderProgram> m_TextureShader;
		std::unique_ptr<ShaderProgram> m_CubeMapShader;
		std::unique_ptr<Camera>        m_Camera;
		std::unique_ptr<CubeMap>	   m_CubeMap;
		std::unique_ptr<Texture>	   m_Texture;

	private:
		void DoPreviousInit();
	};
}