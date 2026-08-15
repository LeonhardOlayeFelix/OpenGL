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

		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		IndexBuffer m_IBO{};
		UniformBuffer m_UBO{};
		ShaderProgram m_ShaderRed{};
		ShaderProgram m_ShaderBlue{};
		ShaderProgram m_ShaderYellow{};
		ShaderProgram m_ShaderGreen{};
		Camera m_Camera{};

		unsigned int m_UBOMatrices;

	private:
		void DoPreviousInit();
	};
}
