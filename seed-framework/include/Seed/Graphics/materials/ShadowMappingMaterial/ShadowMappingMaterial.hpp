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
* \file ShadowMappingMaterial.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef SHADOWMAPPINGMATERIAL_HPP
#define SHADOWMAPPINGMATERIAL_HPP

//SEED INCLUDES
#include <Seed/Graphics/engine/material.hpp>

/*! \class ShadowMappingMaterial
* \brief Material
*/
class ShadowMappingMaterial : public Material
{
	public:

		/*!
		* \brief Constructor of class ShadowMappingMaterial
		* \param scene: address of the scene
		* \param name name of the material
		* \param color Color of the object
		* \param reflection weight of the reflective coefficient
		* \param refraction weight of the refractive coefficient
		* \param flag pointer of an int to get any errors
		*/
		ShadowMappingMaterial(std::shared_ptr<Scene> scene, unsigned int *flag = nullptr);

		~ShadowMappingMaterial();

		void firstPass(Model *model, glm::mat4 &M, glm::mat4 &VP);
		void print(){};

	private:

		void init();

		/*!
		* \brief Load shaders from the directory "Shaders" in directory of this material
		* \param pathDir: path to the directory of the shaders
		*/
		bool addShaders(const std::string pathDir);

		std::shared_ptr<Shader> shader;
		glm::mat4 VPLight, WVPLight;
		GLuint WVPlightID;
};

#endif