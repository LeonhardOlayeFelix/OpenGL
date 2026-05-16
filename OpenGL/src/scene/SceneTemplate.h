#pragma once
#include "Scene.h"

namespace scene {
	class SampleScene : public Scene
	{
	public:
		SampleScene();
		~SampleScene();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}


