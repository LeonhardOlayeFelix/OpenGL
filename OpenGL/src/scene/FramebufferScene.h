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

	class FramebufferScene : public Scene
	{
	public:
		FramebufferScene();
		~FramebufferScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		VertexArray m_VAO{};
		VertexArray m_VAO2{};
		VertexBuffer m_VBO{};
		VertexBuffer m_VBO2{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
		ShaderProgram m_ScreenShader{};
		Texture m_StoneTexture{};
		Texture m_LampTexture{};
		Camera m_Camera{};
		FrameBuffer m_MultisampleFBO{};
		FrameBuffer m_ResolveFBO{};
		float						   m_KernelOffset = 1.0f / 300.0f;
		float						   m_Kernel[9] = { -1, -1, -1,
													   -1,  9, -1,
													   -1, -1, -1 };

	private:
		void DoPreviousInit();
	};
}
