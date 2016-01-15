//SEED INCLUDES
#include <Seed/Graphics/lights/directionnalLight.hpp>

DirectionnalLight::DirectionnalLight(const std::string &n, const glm::vec3 &dir, const glm::vec3 &K2, const glm::vec3 &c) : Light(n, c)
{
	this->direction = dir;
	this->K = K2;
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
