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

		std::unique_ptr<VertexArray>   m_LightVAO;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_ObjectShader;
		std::unique_ptr<ShaderProgram> m_LampShader;
		std::unique_ptr<Texture>       m_Texture;
		std::unique_ptr<Camera>        m_Camera;
		std::unique_ptr<Model>		   m_Model;

		static constexpr int NR_POINT_LIGHTS = 4;

		bool m_AutoMove = false;

	private:
		void DoPreviousInit();
	};
}