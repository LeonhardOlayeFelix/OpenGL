#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"
#include <array>

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

namespace scene {
	class Transformations3DScene : public Scene
	{
	public:
		Transformations3DScene();
		~Transformations3DScene();

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
		glm::mat4 m_ModelTransform;
		glm::mat4 m_ViewTransform;
		glm::mat4 m_ProjTransform;
		std::array<glm::vec3, 10> m_CubePositions;
		float m_TranslateX;
		float m_TranslateY;
		float m_TranslateZ;
		float m_Angle;
		float m_ScaleX;
		float m_ScaleY;
		float m_ScaleZ;
		float m_Fov;

	};
}


