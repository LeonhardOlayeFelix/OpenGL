#include "PointLight.h"

glm::mat4 PointLight::GetProjectionMatrix(float aspect, float near, float far)
{
   return glm::perspective(glm::radians(90.0f), aspect, near, far);
}

std::array<glm::mat4, 6> PointLight::GetViewMatrices()
{
	return {
		glm::lookAt(Position, Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
		glm::lookAt(Position, Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)),
		glm::lookAt(Position, Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
		glm::lookAt(Position, Position + glm::vec3(0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)),
		glm::lookAt(Position, Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)),
		glm::lookAt(Position, Position + glm::vec3(0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)),
	};
}

std::array<glm::mat4, 6> PointLight::GetLightSpaceMatrices(float aspect)
{
	std::array<glm::mat4, 6> viewMatrices = GetViewMatrices();
	glm::mat4 projectionMatrix = GetProjectionMatrix(aspect);

	return {
		projectionMatrix * viewMatrices[0],
		projectionMatrix * viewMatrices[1],
		projectionMatrix * viewMatrices[2],
		projectionMatrix * viewMatrices[3],
		projectionMatrix * viewMatrices[4],
		projectionMatrix * viewMatrices[5],
	};
}
