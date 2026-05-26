#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>

namespace scene {
	class LightingScene : public Scene
	{
	public:
		LightingScene();
		~LightingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexArray> m_LightVAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<ShaderProgram> m_PhongShader;
		std::unique_ptr<ShaderProgram> m_GouraudShader;
		ShaderProgram* m_ObjectShader = nullptr;
		std::unique_ptr<ShaderProgram> m_LampShader;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<Camera> m_Camera;

		glm::vec3 m_LightPosition = glm::vec3(1.2f, 1.0f, -4.0f);
		glm::vec3 m_LightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
		glm::vec3 m_LightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 m_LightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

		bool m_AutoMove = false;

		glm::vec3 m_ObjectScale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_ObjectTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_ObjectRotate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_ObjectAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 m_ObjectDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
		glm::vec3 m_ObjectSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

		float m_Kc = 1.0;
		float m_Kl = 0.09;
		float m_Kq = 0.032;

		float m_Shininess = 32.0f;

		int m_ShadingModel = 0;

	};
}


