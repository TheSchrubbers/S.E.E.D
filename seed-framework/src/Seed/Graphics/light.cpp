#include <Seed/Graphics/light.hpp>

Light::Light(const std::string &n, const glm::vec3 &c)
{
	this->name = n;
	this->color = c;
}

Light::~Light()
{
}

glm::vec3 Light::getColor()
{
	return this->color;
}

std::string Light::getName()
{
	return this->name;
}