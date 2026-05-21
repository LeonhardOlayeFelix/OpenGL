#include "MenuScene.h"
#include "imgui.h"

scene::MenuScene::MenuScene(Scene*& currentScene) : m_CurrentScene(currentScene)
{
}

scene::MenuScene::~MenuScene()
{
}

void scene::MenuScene::OnUpdate(float deltaTime, GLFWwindow* window)
{
}


void scene::MenuScene::OnRender()
{

}

void scene::MenuScene::OnImGuiRender()
{
	for (auto& scene : m_Scenes) 
	{
		if (ImGui::Button(scene.first.c_str())) {
			m_CurrentScene = scene.second();
		}
	}
}

void scene::MenuScene::SetScene(const char* name)
{
	for (auto& scene : m_Scenes) {
		if (scene.first == name)
			m_CurrentScene = scene.second();
	}
}
