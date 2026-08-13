#pragma once
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"


class LightBase
{
public:
	glm::vec3 Ambient{}, Diffuse{}, Specular{};
protected:
	LightBase() = default;
	LightBase(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Ambient(ambient), Diffuse(diffuse), Specular(specular) {}
};
