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
		static constexpr int NR_OBJECTS = 100;


		GLFWwindow* m_Window;

		VertexArray m_VAO{};
		VertexBuffer m_VBO{};
		VertexBuffer m_InstanceVBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_Shader{};
		Camera m_Camera{};

		glm::vec3 translations[NR_OBJECTS];

	private:
		void DoPreviousInit();
	};
}


