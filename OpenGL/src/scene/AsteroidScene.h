#pragma once
#include "Scene.h"
#include "Common.h"
#include "Model.h"
#include <memory>


namespace scene{
	class AsteroidScene : public Scene
	{
	public:
		AsteroidScene();
		~AsteroidScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		GLFWwindow* m_Window;
		static constexpr int NR_ASTEROIDS = 1000;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexBuffer> m_InstanceVBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<ShaderProgram> m_Shader;
		std::unique_ptr<Camera>        m_Camera;

		glm::mat4 m_ModelMatrices[NR_ASTEROIDS];
		
		std::unique_ptr<Model> m_Planet;
		std::unique_ptr<Model> m_Rock;

		float m_Radius = 50.0f;
		float m_Offset = 2.5f;


	private:
		void DoPreviousInit();
		void RandomiseModels();
	};
}


