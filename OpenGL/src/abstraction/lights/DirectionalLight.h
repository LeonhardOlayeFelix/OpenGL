#pragma once
#include "LightBase.h"
#include "Directional.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

/*
* * * * FOR SHADERS * * * *
struct DirectionalLight
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Direction;
};
*/

class DirectionalLight : public LightBase, public Directional
{
public:
    DirectionalLight() = default;
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) : LightBase(ambient, diffuse, specular), Directional(direction) {};
};

