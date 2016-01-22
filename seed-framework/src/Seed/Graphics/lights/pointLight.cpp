//SEED INCLUDES
#include <Seed/Graphics/lights/pointLight.hpp>
#include <Seed/Graphics/engine/Constant.hpp>

PointLight::PointLight(const std::string &n, const glm::vec3 &p, const glm::vec3 &K2, const glm::vec3 &c, const float &near, const float &far) : Light(n, c)
{
	this->position = p;
	this->constant = Attenuation[0][distToAttenuation.at(far)];
	this->linear = Attenuation[1][distToAttenuation.at(far)];
	this->quadratic = Attenuation[2][distToAttenuation.at(far)];
	this->K = K2;
	this->V = glm::lookAt(p, glm::vec3(0.0), glm::vec3(0.0,1.0,0.0));
	this->P = glm::perspective(
		180.0f,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		near,        // la ou commence le frustrum
		far// la ou finit le frustrum
		);
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