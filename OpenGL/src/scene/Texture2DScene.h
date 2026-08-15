#pragma once
#include "Scene.h"
#include "Common.h"
#include <memory>


namespace scene{
	class Texture2DScene : public Scene
	{
	public:
		Texture2DScene();
		~Texture2DScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
		Texture m_Texture{};
		Texture m_Texture2{};
		float m_float;
		float m_float2;
	};
}


