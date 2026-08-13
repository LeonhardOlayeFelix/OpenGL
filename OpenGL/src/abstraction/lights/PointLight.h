#pragma once
#include "Positional.h"
#include "Attenuated.h"

class PointLight : public LightBase, public Positional, public Attenuated
{
public:
	PointLight() = default;
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float kc, float kl, float kq) : LightBase(ambient, diffuse, specular), Positional(position), Attenuated(kc, kl, kq){};
public:
	glm::vec3 Position{};
};

