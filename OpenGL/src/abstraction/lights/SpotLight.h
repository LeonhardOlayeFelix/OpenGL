#pragma once
#include "Attenuated.h"
#include "Directional.h"
#include "Positional.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

class SpotLight : public LightBase, public Attenuated, public Positional, public Directional
{
public:
	float CutOff{}, OuterCutOff{};
public:
	SpotLight() = default;
	SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float kc, float kl, float kq, float cutOff, float outerCutOff) : LightBase(ambient, diffuse, specular), Attenuated(kc, kl, kq), Positional(position), Directional(direction), CutOff(cutOff), OuterCutOff(outerCutOff) {};
};

