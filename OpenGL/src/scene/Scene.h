#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace scene {
	class Scene
	{
	public:
		Scene();
		virtual ~Scene() {}

		virtual void OnUpdate(double deltaTime, GLFWwindow* window) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};
}


