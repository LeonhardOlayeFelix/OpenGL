#pragma once
#include "Scene.h"

namespace scene {
	class ClearColourScene : public Scene
	{
	public:
		ClearColourScene();
		~ClearColourScene() override;

		void OnUpdate(double deltaTime, GLFWwindow* window) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}


