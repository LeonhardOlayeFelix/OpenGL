#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>

namespace scene {
	class Camera3DScene : public Scene
	{
	public:
		Camera3DScene();
		~Camera3DScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		std::array<glm::vec3, 10> m_CubePositions;

		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_IBO;
		ShaderProgram m_Shader;
		Texture m_Texture;
		Texture m_Texture2;
		Camera m_Camera;
	};
}


