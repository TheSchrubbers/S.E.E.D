//SEED INCLUDES
#include <Seed/Graphics/engine/camera.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float n, float f , float WA, float HA, float s, float ms)
{
	this->position = pos;
	this->direction = glm::normalize(pos - look);
	this->up = glm::normalize(upVector);
	this->WAngle = WA;
	this->HAngle = HA;
	this->FoV = f;
	this->near = n;
	this->far = f;
	this->speed = s;
	this->mouseSpeed = ms;
	this->right = glm::normalize(glm::cross(this->up, this->direction));
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

glm::vec3 Camera::getPosition()
{
	return this->position;
}

glm::vec3 Camera::getDirection()
{
	return this->direction;
}

glm::vec3 Camera::getUp()
{
	return this->up;
}

float Camera::getWAngle()
{
	return this->WAngle;
}

float Camera::getHAngle()
{
	return this->HAngle;
}

float Camera::getNear()
{
	return this->near;
}

float Camera::getFar()
{
	return this->far;
}

float Camera::getSpeed()
{
	return this->speed;
}

float Camera::getMouseSpeed()
{
	return this->mouseSpeed;
}

float Camera::getFoV()
{
	return this->FoV;
}

const float Camera::getInitFoV()
{
	return this->initFoV;
}

void Camera::setHAngle(float HA)
{
	this->HAngle = HA;
}

void Camera::setWAngle(float WA)
{
	this->WAngle = WA;
}


void Camera::setProjectionMatrix(const float &F, const int width, const int height, const float &n, const float &fa)
{
	this->FoV = F;
	this->near = n;
	this->far = fa;
	projectionMatrix = glm::perspective(F, (float)width / (float)height, n, fa);
}

void Camera::setViewMatrix(glm::vec3 &pos, glm::vec3 &dir, glm::vec3 &u)
{
	this->position = pos;
	this->direction = dir;
	this->up = u;
	viewMatrix = glm::lookAt(pos, pos + dir, u);
}

void Camera::setMouseSpeed(const float speed)
{
	this->mouseSpeed = speed;
}

glm::vec3 Camera::getRight()
{
	return this->right;
}

void Camera::setRight(glm::vec3 &r)
{
	this->right = r;
}

void Camera::setViewMatrix(glm::vec3 &pos)
{
	this->position = pos;
	viewMatrix = glm::lookAt(pos, pos + this->direction, this->up);
}