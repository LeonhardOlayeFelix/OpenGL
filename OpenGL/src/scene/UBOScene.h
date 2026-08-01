#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include "Model.h"

namespace scene {

	class UBOScene : public Scene
	{
	public:
		UBOScene();
		~UBOScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<UniformBuffer> m_UBO;
		std::unique_ptr<ShaderProgram> m_ShaderRed;
		std::unique_ptr<ShaderProgram> m_ShaderBlue;
		std::unique_ptr<ShaderProgram> m_ShaderYellow;
		std::unique_ptr<ShaderProgram> m_ShaderGreen;
		std::unique_ptr<Camera>        m_Camera;

		unsigned int m_UBOMatrices;

	private:
		void DoPreviousInit();
	};
}