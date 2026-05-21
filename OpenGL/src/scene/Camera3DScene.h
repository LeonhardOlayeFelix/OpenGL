#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

namespace scene {
	class Camera3DScene : public Scene
	{
	public:
		Camera3DScene();
		~Camera3DScene();

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
		glm::mat4 m_ModelTransform;
		glm::mat4 m_ViewTransform;
		glm::mat4 m_ProjTransform;
		std::array<glm::vec3, 10> m_CubePositions;
		GLFWwindow* m_Window;
		float m_TranslateX = 0.0f;
		float m_TranslateY = 0.0f;
		float m_TranslateZ = 0.0f;
		float m_Angle = 0.0f;
		float m_ScaleX = 1.0f;
		float m_ScaleY = 1.0f;
		float m_ScaleZ = 1.0f;
		float m_Fov = 45.0f;
		float m_Yaw = -90.0f;
		float m_Pitch;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	private:
		inline glm::vec3 GetCameraFront() 
		{
			//Euler angles
			return glm::normalize(glm::vec3(
				cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
				sin(glm::radians(m_Pitch)),
				sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
			));

		}
	};
}


