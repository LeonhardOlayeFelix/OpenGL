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

	class FaceCullingScene : public Scene
	{
	public:
		FaceCullingScene();
		~FaceCullingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;
		 
		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
		Texture m_Texture{};
		Camera m_Camera{};

	private:
		void DoPreviousInit();
	};
}
