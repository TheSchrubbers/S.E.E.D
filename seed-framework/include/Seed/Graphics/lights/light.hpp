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
* \file light.hpp
* \brief Lights of the scene
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <map>

/*! \class Light
* \brief Light of the scene
*/
class Light
{
public:
	/*!
	* \brief Constructor of class Light
	* \param name name of the light
	* \param color color of the light
	* \param isSendingShadow bbol if the light is sending shadows
	*/
	Light(const std::string &name, const glm::vec3 &color, const bool isSendingShadow = true);
	/*!
	* \brief Destructor of class Light
	*/
	~Light();
	/*!
	* \brief Get color of the light
	* \return Color of the light
	*/
	glm::vec3 getColor();
	/*!
	* \brief Get the bool if the light sent shadows
	* \return bool if the light sent shadows
	*/
	bool isSendShadow();
	/*!
	* \brief Set light if it's sending shadow
	* \param sendShadow bool
	*/
	void setShadow(bool isSendingShadow);
	/*!
	* \brief get the name of the light
	* \return the name of hte light
	*/
	std::string getName();
	/*!
	* \brief get viewMatrix of the light
	* \return viewMatrix of the light
	*/
	glm::mat4 getV();
	/*!
	* \brief get projectionMatrix of the light
	* \return projectionMatrix of the light
	*/
	glm::mat4 getP();

	virtual void afficher() = 0;

protected:
	std::string name;
	glm::vec3 color;
	bool shadow;
	glm::mat4 V, P;
	
};

#endif