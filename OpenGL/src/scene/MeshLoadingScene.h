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

	class MeshLoadingScene : public Scene
	{
	public:
		MeshLoadingScene();
		~MeshLoadingScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		GLFWwindow* m_Window;

		VertexArray m_LightVAO{};
		VertexBuffer m_VBO{};
		IndexBuffer m_IBO{};
		ShaderProgram m_ObjectShader{};
		ShaderProgram m_LampShader{};
		Texture m_Texture{};
		Camera m_Camera{};
		Model m_Model{};

		glm::vec3 m_PointLightPosition = glm::vec3(3.0f);
		glm::vec3 m_PointLightAmbient = glm::vec3(0.1f);
		glm::vec3 m_PointLightDiffuse = glm::vec3(0.7f);
		glm::vec3 m_PointLightSpecular = glm::vec3(1.0f);
		float m_PointLightKc = 1.0f;
		float m_PointLightKl = 0.09f;
		float m_PointLightKq = 0.1f;

		static constexpr int NR_POINT_LIGHTS = 4;

		bool m_AutoMove = false;

	private:
		void DoPreviousInit();
	};
}
