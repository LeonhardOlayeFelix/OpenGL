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

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		

		template<typename T>
		void RegisterScene(const char* name) 
		{
           std::function<Scene* ()> f = [name]() { return new T(name); };
			m_Scenes.push_back({ name, f });
		}

	private:
		Scene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<Scene* ()>>> m_Scenes;
	};
}


