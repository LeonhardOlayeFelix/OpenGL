#pragma once
#include "Scene.h"
#include "Common.h"
#include <memory>


namespace scene{
	class TriangleScene : public Scene
	{
	public:
		TriangleScene();
		~TriangleScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
	};
}


