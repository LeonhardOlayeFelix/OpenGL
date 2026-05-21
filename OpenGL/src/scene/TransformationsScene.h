#pragma once
#include <memory>

#include "Scene.h"
#include "Common.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

namespace scene {
	class TransformationsScene : public Scene
	{
	public:
		TransformationsScene();
		~TransformationsScene();

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
		float m_TranslateX;
		float m_TranslateY;
		float m_TranslateZ;
		float m_Angle;
		float m_ScaleX;
		float m_ScaleY;
		float m_ScaleZ;
	};
}


