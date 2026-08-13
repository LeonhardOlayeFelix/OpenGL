#pragma once
#include "LightBase.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

class Directional
{
protected:
	Directional() = default;
	Directional(glm::vec3 direction) : Direction(direction) {};
public: 
	glm::vec3 Direction;
};

