#pragma once
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"


class Positional
{
protected:
	Positional() = default;
	Positional(glm::vec3 position) : Position(position) {};
public:
	glm::vec3 Position{};

};

