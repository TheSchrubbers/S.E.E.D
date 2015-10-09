/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file pointLight.hpp
* \brief Pointslights of the scene
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <Seed/Graphics/light.hpp>

/*! \class PointLight
* \brief PointLight of the scene
*/
class PointLight : public Light
{
public:
	/*!
	* \brief Constructor of class PointLight
	* \param name: name of the light
	* \param color: color of the light
	* \param position: position of the light
	*/
	PointLight(const std::string &name, const glm::vec3 &position, const glm::vec3 &color = glm::vec3(1.0), const int &distance = 50);
	/*!
	* \brief Destructor of class PointLight
	*/
	~PointLight();
	/*!
	* \brief Get position of the light
	* \return Position of the light
	*/
	glm::vec3 getPosition();
	/*!
	* \brief get the attenuation of the light
	* \return the constant, the linear and the quadratic param in a glm::vec3 int this order
	*/
	glm::vec3 getAttenuation();

	virtual void afficher(){};

private:
	glm::vec3 position;
	float constant, quadratic, linear;
};

//structure for UBO of light
struct pointLightStruct
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 attenuation;
	glm::ivec4 size;
};

#endif