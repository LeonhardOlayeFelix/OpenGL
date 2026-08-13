#pragma once
#include "Positional.h"
#include "Attenuated.h"

/*
* * * * FOR SHADERS * * * *
struct PointLight        
{                        
    vec3 Position;       
    vec3 Ambient;        
    vec3 Diffuse;        
    vec3 Specular;       
    float Kc;            
    float Kl;            
    float Kq; 
};                       
*/

class PointLight : public LightBase, public Positional, public Attenuated
{
public:
	PointLight() = default;
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, float kc, float kl, float kq) : LightBase(ambient, diffuse, specular), Positional(position), Attenuated(kc, kl, kq){};
};

