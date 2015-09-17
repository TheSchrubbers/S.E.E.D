#include <Seed/Graphics/camera.hpp>

//#include "..\..\..\include\Seed\Graphics\camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float near, float far)
{
	viewMatrix = glm::lookAt(pos, look, upVector);
	projectionMatrix = glm::perspective(
		FOV,         //angle d'ouverture de la caméra
		width/height, // ratio de la résolution de l'ecran
		near,        // la ou commence le frustrum
		far       // la ou finit le frustrum
	);
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::setProjectionMatrix(const float &Fov, const int width, const int height, const float &near, const float &far)
{
	projectionMatrix = glm::perspective(Fov, (float)width / (float)height, near, far);
}

void Camera::setViewMatrix(glm::vec3 & pos, glm::vec3 &dir, glm::vec3 &up)
{
	viewMatrix = glm::lookAt(pos, pos + dir, up);
}