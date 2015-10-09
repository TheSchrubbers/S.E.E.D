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
* \file directionnalLight.hpp
* \brief Directionnallights of the scene
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef DIRECTIONNALLIGHT_HPP
#define DIRECTIONNALLIGHT_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <Seed/Graphics/light.hpp>

/*! \class DirectionnalLight
* \brief DirectionnalLight of the scene
*/
class DirectionnalLight : public Light
{
public:
	/*!
	* \brief Constructor of class DirectionnalLight
	* \param name: name of the light
	* \param color: color of the light
	* \param direction: direction of the light
	*/
	DirectionnalLight(const std::string &name, const glm::vec3 &direction, const glm::vec3 &color = glm::vec3(1.0));
	/*!
	* \brief Destructor of class DirectionnalLight
	*/
	~DirectionnalLight();
	void afficher(){}

	/*!
	* \brief get the direction of the light
	* \return the direction of the light
	*/
	glm::vec3 getDirection();

private:
	glm::vec3 direction;
};

//structure for UBO of light
struct directionnalLightStruct
{
	glm::vec4 direction;
	glm::vec4 color;
	glm::ivec4 size;
};

#endif