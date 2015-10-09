#include <Seed/Graphics/light/directionnalLight.hpp>

DirectionnalLight::DirectionnalLight(const std::string &n, const glm::vec3 &dir, const glm::vec3 &c) : Light(n, c)
{
	this->direction = dir;
}

DirectionnalLight::~DirectionnalLight()
{
}

glm::vec3 DirectionnalLight::getDirection()
{
	return this->direction;
}
