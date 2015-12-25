#include <Seed/Graphics/light/pointLight.hpp>
#include <Seed/Graphics/Constant.hpp>

PointLight::PointLight(const std::string &n, const glm::vec3 &p, const glm::vec3 &K2, const glm::vec3 &c, const int &dist) : Light(n, c)
{ 
	this->position = p;
	this->constant = Attenuation[0][distToAttenuation.at(dist)];
	this->linear = Attenuation[1][distToAttenuation.at(dist)];
	this->quadratic = Attenuation[2][distToAttenuation.at(dist)];
	this->K = K2;
}

PointLight::~PointLight()
{
}

glm::vec3 PointLight::getPosition()
{
	return this->position;
}

glm::vec3 PointLight::getAttenuation()
{
	return glm::vec3(constant, linear, quadratic);
}

glm::vec3 PointLight::getK()
{
	return this->K;
}