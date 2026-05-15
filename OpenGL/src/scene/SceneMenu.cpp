#include "SceneMenu.h"
#include "imgui.h"

scene::SceneMenu::SceneMenu(Scene*& currentScene) : Scene("Menu"), m_CurrentScene(currentScene)
{
}

scene::SceneMenu::~SceneMenu()
{
}

void scene::SceneMenu::OnUpdate(float deltaTime)
{
}

void scene::SceneMenu::OnRender()
{
}

void scene::SceneMenu::OnImGuiRender()
{
	for (auto& scene : m_Scenes) 
	{
		if (ImGui::Button(scene.first.c_str())) {
			m_CurrentScene = scene.second();
		}
	}
}
