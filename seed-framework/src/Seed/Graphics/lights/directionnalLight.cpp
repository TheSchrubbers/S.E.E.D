//SEED INCLUDES
#include <Seed/Graphics/lights/directionnalLight.hpp>

DirectionnalLight::DirectionnalLight(const std::string &n, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &K2, const glm::vec3 &c, const float &near, const float &far) : Light(n, c)
{
	this->direction = dir;
	this->K = K2;
	this->V = glm::lookAt(pos, pos + dir, glm::vec3(0.0,1.0,0.0));
	this->P = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
}

DirectionnalLight::~DirectionnalLight()
{
}

glm::vec3 DirectionnalLight::getDirection()
{
	return this->direction;
}

glm::vec3 DirectionnalLight::getK()
{
	return this->K;
}
