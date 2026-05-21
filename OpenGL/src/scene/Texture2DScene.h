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

		void OnUpdate(float deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Texture> m_Texture2;
		float m_float;
		float m_float2;
	};
}


