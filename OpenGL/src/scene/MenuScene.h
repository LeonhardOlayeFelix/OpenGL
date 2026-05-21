#pragma once
#include "Scene.h"
#include <vector>
#include <string>
#include <functional>

namespace scene {
	class MenuScene : public Scene
	{
	public:
		MenuScene(Scene*& currentScene);
		~MenuScene();

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;

		

		template<typename T>
		void RegisterScene(const char* name) 
		{
           std::function<Scene* ()> f = []() { return new T(); };
			m_Scenes.push_back({ name, f });
		}

		void SetScene(const char* name);

	private:
		Scene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<Scene* ()>>> m_Scenes;
	};
}


