#pragma once
#include "Scene.h"

namespace scene {
	class Texture2DScene : public Scene
	{
	public:
		Texture2DScene();
		~Texture2DScene() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}


