#include <Seed/Graphics/light.hpp>

Light::Light( glm::vec3 c, glm::vec3 p)
{
	this->color = c;
	this->position = p;
}

Light::~Light()
{
}

void Light::render()
{

}