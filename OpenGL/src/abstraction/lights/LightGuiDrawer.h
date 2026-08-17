#pragma once
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <type_traits>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "LightBase.h"
#include "Directional.h"
#include "Attenuated.h"
#include "Positional.h"
#include "SpotLight.h"

class LightGuiDrawer
{
public:
	static void DrawLightBaseFields(LightBase& light);
	static void DrawPositionalFields(Positional& light);
	static void DrawAttenuatedFields(Attenuated& light);
	static void DrawDirectionalFields(Directional& light);
	static void DrawCutOffFields(SpotLight& light);

	template<typename T>
	static void DrawObjectInspector(T& light, const char* windowName) {

		ImGui::Begin(windowName);

		DrawLightBaseFields(light);

		if constexpr (std::is_base_of_v<Positional, T>)
			DrawPositionalFields(light);

		if constexpr (std::is_base_of_v<Directional, T>)
			DrawDirectionalFields(light);

		if constexpr (std::is_base_of_v<Attenuated, T>)
			DrawAttenuatedFields(light);

		if constexpr (std::is_base_of_v<SpotLight, T>)
			DrawCutOffFields(light);

		ImGui::End();
	}

};

