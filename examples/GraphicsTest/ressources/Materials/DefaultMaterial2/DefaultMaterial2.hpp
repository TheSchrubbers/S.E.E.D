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
* \file DefaultMaterial2.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef DEFAULTMATERIAL2_HPP
#define DEFAULTMATERIAL2_HPP

#include <Seed/Graphics/material.hpp>

/*! \class DefaultMaterial2
* \brief Material
*/
class DefaultMaterial2 : public Material
{
public:

	/*!
	* \brief Constructor of class DefaultMaterial2
	* \param material: address of the aimaterial
	* \param camera: address of the camera in the scene
	* \param name: name of the material
	*/
	DefaultMaterial2(const aiMaterial *material, Camera *camera, std::string name, unsigned int *flag = NULL);
	/*!
	* \brief Constructor of class DefaultMaterial2
	* \param camera: address of the camera in the scene
	* \param name: name of the material
	* \param pathShaders: path to the directory who contains shaders' files
	*/
	DefaultMaterial2(Camera *camera, const std::string name, unsigned int *flag = NULL);

	~DefaultMaterial2();

	void render(Model *model);

private:
	glm::mat4 M;
};

#endif