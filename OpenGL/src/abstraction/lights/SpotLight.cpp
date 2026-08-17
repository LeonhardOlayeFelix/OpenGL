#include "SpotLight.h"

glm::mat4 SpotLight::GetProjectionMatrix()
{
    float fov = 2.0f * glm::radians(OuterCutOff);
    return glm::perspective(fov, 1.0f, Near, Far);
}

glm::mat4 SpotLight::GetViewMatrix()
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glm::abs(glm::dot(glm::normalize(Direction), up)) > 0.99f)
        up = glm::vec3(0.0f, 0.0f, 1.0f);

    return glm::lookAt(Position, Position + Direction, up);
}

glm::mat4 SpotLight::GetLightSpaceMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}
