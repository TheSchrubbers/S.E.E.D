//SEED INCLUDES
#include <Seed/Graphics/lights/spotLight.hpp>
#include <Seed/Graphics/engine/Constant.hpp>

SpotLight::SpotLight(const std::string &n, const glm::vec3 &p, const glm::vec3 &dir, const glm::vec3 &K2, const glm::vec3 &c,const float &ang, const float &near, const float &far, const float &att) : Light(n, c)
{
	this->position = p;
	this->direction = dir;
	this->angle = ang;
	//this->constant = Attenuation[0][distToAttenuation.at(far)];
	//this->linear = Attenuation[1][distToAttenuation.at(far)];
	//this->quadratic = Attenuation[2][distToAttenuation.at(far)];
	this->attenuation = att;
	this->K = K2;
	this->V = glm::lookAt(p, p + dir, glm::vec3(0.0,1.0,0.0));
	this->P = glm::perspective(
		ang,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		near,        // la ou commence le frustrum
		far// la ou finit le frustrum
		);
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

float SpotLight::getAttenuation()
{
	return this->attenuation;
}

float SpotLight::getAngle()
{
	return this->angle;
}

glm::vec3 SpotLight::getK()
{
	return this->K;
}