//SEED INCLUDES
#include <Seed/Graphics/lights/light.hpp>

Light::Light(const std::string &n, const glm::vec3 &c, const bool isSendingShadows)
{
	this->name = n;
	this->color = c;
	this->shadow = isSendingShadows;
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

bool Light::isSendShadow()
{
	return this->shadow;
}

void Light::setShadow(bool s)
{
	this->shadow = s;
}

glm::mat4 Light::getP()
{
	return this->P;
}

glm::mat4 Light::getV()
{
	return this->V;
}
