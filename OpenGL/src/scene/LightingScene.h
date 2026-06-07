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

namespace scene {

	struct DirLightData
	{
		glm::vec3 direction;
		glm::vec3 ambient ;
		glm::vec3 diffuse ;
		glm::vec3 specular ;
		/*glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
		glm::vec3 ambient = glm::vec3(0.17f, 0.17f, 0.17f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);*/
	};

	struct PointLightData
	{
		glm::vec3 position;
		glm::vec3 ambient ;
		glm::vec3 diffuse ;
		glm::vec3 specular;
		/*glm::vec3 position = glm::vec3(1.2f, 1.0f, -2.0f);
		glm::vec3 ambient = glm::vec3(0.17f, 0.17f, 0.17f);
		glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);*/
		float Kc = 1.0f;
		float Kl = 0.09f;
		float Kq = 0.032f;
	};
	struct SpotLightData
	{
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff = 12.5;
		float outerCutOff = 17.5;
		float Kc = 1.0f;
		float Kl = 0.09f;
		float Kq = 0.1f;
		glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 diffuse = glm::vec3(0.2f, 0.5f, 0.1f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	};
	

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

		std::unique_ptr<VertexArray>   m_VAO;
		std::unique_ptr<VertexArray>   m_LightVAO;
		std::unique_ptr<VertexBuffer>  m_VBO;
		std::unique_ptr<IndexBuffer>   m_IBO;
		std::unique_ptr<ShaderProgram> m_ObjectShader;
		std::unique_ptr<ShaderProgram> m_LampShader;
		std::unique_ptr<Texture>       m_Texture;
		std::unique_ptr<Texture>       m_Texture2;
		std::unique_ptr<Texture>       m_Texture3;
		std::unique_ptr<Camera>        m_Camera;

		static constexpr int NR_POINT_LIGHTS = 4;

		DirLightData   m_DirLight;
		PointLightData m_PointLights[NR_POINT_LIGHTS];
		SpotLightData m_SpotLight;

		bool m_AutoMove = false;

		glm::vec3 m_ObjectScale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_ObjectTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_ObjectRotate = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_ObjectSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

		float m_Shininess = 32.0f;
		int   m_ShadingModel = 0;
	};
}