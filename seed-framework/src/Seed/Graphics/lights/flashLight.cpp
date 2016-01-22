//SEED INCLUDES
#include <Seed/Graphics/lights/flashLight.hpp>
#include <Seed/Graphics/engine/Constant.hpp>

FlashLight::FlashLight(const std::string &n, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &K2, const glm::vec3 &c, const float &ang, const float &near, const float &far) : Light(n, c)
{
	this->position = pos;
	this->direction = dir;
	this->angle = ang;
	this->constant = Attenuation[0][distToAttenuation.at(far)];
	this->linear = Attenuation[1][distToAttenuation.at(far)];
	this->quadratic = Attenuation[2][distToAttenuation.at(far)];
	this->K = K2;
	this->V = glm::lookAt(pos, pos + dir, glm::vec3(0.0,1.0,0.0));
	this->P = glm::perspective(
		ang,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		near,        // la ou commence le frustrum
		far// la ou finit le frustrum
		);
}

FlashLight::~FlashLight()
{
}

glm::vec3 FlashLight::getAttenuation()
{
	return glm::vec3(this->constant, this->linear, this->quadratic);
}

glm::vec3 FlashLight::getDirection()
{
	return this->direction;
}

glm::vec3 FlashLight::getPosition()
{
	return this->position;
}

glm::vec3 FlashLight::getK()
{
	return this->K;
}