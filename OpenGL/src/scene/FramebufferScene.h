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

		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexArray>   m_VAO2;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<VertexBuffer>  m_VBO2;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<ShaderProgram> m_ScreenShader;
		std::unique_ptr<Texture>       m_StoneTexture;
		std::unique_ptr<Texture>       m_LampTexture;
		std::unique_ptr<Camera>        m_Camera;
		std::unique_ptr<FrameBuffer>   m_FBO2;
		unsigned int				   m_FBO;
		unsigned int				   m_ColorBufferAttachment;
		unsigned int				   m_DepthAndStencilBuffer;
		float						   m_KernelOffset = 1.0f / 300.0f;
		float						   m_Kernel[9] = { -1, -1, -1,
													   -1,  9, -1,
													   -1, -1, -1 };

	private:
		void DoPreviousInit();
	};
}