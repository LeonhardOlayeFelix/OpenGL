#pragma once
#include "Scene.h"

namespace scene {
	class ClearColourScene : public Scene
	{
	public:
		ClearColourScene();
		ClearColourScene(const char* name);
		~ClearColourScene();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}


