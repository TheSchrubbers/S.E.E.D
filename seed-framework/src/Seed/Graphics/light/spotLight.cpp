#include <Seed/Graphics/light/spotLight.hpp>
#include <Seed/Graphics/Constant.hpp>

SpotLight::SpotLight(const std::string &n, const glm::vec3 &p, const glm::vec3 &dir, const glm::vec3 &c,const float &ang,const int &dist) : Light(n, c)
{
	this->position = p;
	this->direction = dir;
	this->angle = ang;
	this->constant = Attenuation[0][distToAttenuation.at(dist)];
	this->linear = Attenuation[1][distToAttenuation.at(dist)];
	this->quadratic = Attenuation[2][distToAttenuation.at(dist)];
}

SpotLight::~SpotLight()
{
}

glm::vec3 SpotLight::getPosition()
{
	return this->position;
}

glm::vec3 SpotLight::getDirection()
{
	return this->direction;
}

glm::vec3 SpotLight::getAttenuation()
{
	return glm::vec3(this->constant, this->linear, this->quadratic);
}

float SpotLight::getAngle()
{
	return this->angle;
}