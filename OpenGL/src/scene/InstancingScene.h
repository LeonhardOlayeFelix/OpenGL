#pragma once
#include "Scene.h"
#include "Common.h"
#include <memory>


namespace scene{
	class InstancingScene : public Scene
	{
	public:
		InstancingScene();
		~InstancingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<Camera>        m_Camera;

	private:
		void DoPreviousInit();
	};
}


