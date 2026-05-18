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

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
	};
}


