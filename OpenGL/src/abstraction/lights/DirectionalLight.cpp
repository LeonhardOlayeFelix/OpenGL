#include "DirectionalLight.h"

glm::mat4 DirectionalLight::GetProjectionMatrix(double left, double right, double bottom, double top)
{
    return glm::ortho(left, right, bottom, top, (double)Near, (double)Far);
}

glm::mat4 DirectionalLight::GetViewMatrix(glm::vec3 center, float distanceAway)
{
    glm::vec3 lightPos = center - glm::normalize(Direction) * distanceAway;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glm::abs(glm::dot(glm::normalize(Direction), up)) > 0.99f)
        up = glm::vec3(0.0f, 0.0f, 1.0f);

    return glm::lookAt(lightPos, center, up);
}

glm::mat4 DirectionalLight::GetLightSpaceMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}
