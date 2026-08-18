#include "LightGuiDrawer.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

void LightGuiDrawer::DrawLightBaseFields(LightBase& light)
{
	if (ImGui::CollapsingHeader("LightBase"))
	{
		ImGui::ColorEdit3("Ambient", glm::value_ptr(light.Ambient));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(light.Diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(light.Specular));
		ImGui::SliderFloat("Near Plane", &light.Near, 0.1, light.Far);
		ImGui::SliderFloat("Far Plane", &light.Far, light.Near, 70.0);
	}
}

void LightGuiDrawer::DrawPositionalFields(Positional& light)
{
	if (ImGui::CollapsingHeader("Positional"))
	{
		ImGui::SliderFloat3("Position", glm::value_ptr(light.Position), -10, 10);
	}
}

void LightGuiDrawer::DrawAttenuatedFields(Attenuated& light)
{
	if (ImGui::CollapsingHeader("Attenuated"))
	{
		ImGui::SliderFloat("Constant (Kc)", &light.Kc, 0.0f, 2.0f);
		ImGui::SliderFloat("Linear (Kl)", &light.Kl, 0.0f, 1.0f);
		ImGui::SliderFloat("Quadratic (Kq)", &light.Kq, 0.0f, 1.0f);
	}
}

void LightGuiDrawer::DrawDirectionalFields(Directional& light)
{
	if (ImGui::CollapsingHeader("Directional"))
	{
		ImGui::SliderFloat3("Direction", glm::value_ptr(light.Direction), -10, 10);
	}
}

void LightGuiDrawer::DrawCutOffFields(SpotLight& light)
{
	if (ImGui::CollapsingHeader("Spot Light Parameters"))
	{
		ImGui::SliderFloat("CutOff", &light.CutOff, 1.0f, light.OuterCutOff);
		ImGui::SliderFloat("OuterCutOff", &light.OuterCutOff, light.CutOff, 89.0f);
	}
}
