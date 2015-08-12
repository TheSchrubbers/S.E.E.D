#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float near, float far)
{
	View_matrix = glm::lookAt(pos, look, upVector);
	Projection_matrix = glm::perspective(
		FOV,         //angle d'ouverture de la caméra
		width/height, // ratio de la résolution de l'ecran
		near,        // la ou commence le frustrum
		far       // la ou finit le frustrum
	);
}

glm::mat4 Camera::getViewMatrix()
{
	return View_matrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return Projection_matrix;
}

void Camera::setProjectionMatrix(const float &Fov, const int width, const int height, const float &near, const float &far)
{
	Projection_matrix = glm::perspective(Fov, (float)width / (float)height, near, far);
}

void Camera::setViewMatrix(glm::vec3 & pos, glm::vec3 &dir, glm::vec3 &up)
{
	View_matrix = glm::lookAt(pos, pos + dir, up);
}