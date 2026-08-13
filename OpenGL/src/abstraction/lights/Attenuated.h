#pragma once
#include "LightBase.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

class Attenuated
{
protected:
	Attenuated() = default;
	Attenuated(float kc, float kl, float kq) : Kc(kc), Kl(kl), Kq(kq) {};
	
public:
	float Kc{ 1.0f }, Kl{ 0.09f }, Kq{ 0.1f };
};

