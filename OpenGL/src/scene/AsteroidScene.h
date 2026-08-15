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

		VertexArray   m_VAO;
		VertexBuffer  m_VBO;
		VertexBuffer  m_InstanceVBO;
		IndexBuffer   m_IBO;
		ShaderProgram m_Shader;
		ShaderProgram m_Shader2;
		Camera        m_Camera;

		glm::mat4 m_ModelMatrices[NR_ASTEROIDS];
		
		Model m_Planet;
		Model m_Rock;

		float m_Radius = 50.0f;
		float m_Offset = 10.0f;


	private:
		void DoPreviousInit();
		void RandomiseModels();
	};
}


