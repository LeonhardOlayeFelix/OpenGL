#pragma once
#include <array>
#include "Positional.h"
#include "Attenuated.h"

/*
* * * * FOR SHADERS * * * *
struct PointLight        
{                        
    vec3 Position;       
    vec3 Ambient;        
    vec3 Diffuse;   
    float Near;
    float Far;
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
    glm::mat4 GetProjectionMatrix(float aspect);
    std::array<glm::mat4, 6> GetViewMatrices();
    std::array<glm::mat4, 6> GetLightSpaceMatrices(float aspect = 1.0f);
};

