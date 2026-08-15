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

	class GeometryShaderScene : public Scene
	{
	public:
		GeometryShaderScene();
		~GeometryShaderScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		UniformBuffer m_UBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_ExplosionShader{};
		ShaderProgram m_Shader{};
		ShaderProgram m_NormalShader{};
		Camera m_Camera{};
		Model m_Model{};

		float						   m_Magnitude = 0;
	private:
		void DoPreviousInit();
	};
}
