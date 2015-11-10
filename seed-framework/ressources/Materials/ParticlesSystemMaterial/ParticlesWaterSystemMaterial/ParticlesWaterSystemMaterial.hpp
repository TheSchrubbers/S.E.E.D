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
* \file DefaultMaterial.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef PARTICLESWATERSYSTEMMATERIAL_HPP
#define PARTICLESWATERSYSTEMMATERIAL_HPP

#include <Seed/Graphics/material.hpp>

/*! \class ParticlesWaterSystemMaterial
* \brief Material
*/
class ParticlesWaterSystemMaterial : public Material
{
	public:

		/*!
		* \brief Constructor of class ParticlesWaterSystemMaterial
		* \param scene: address of the scene
		* \param name: name of the material
		* \param flag: pointer of an int to get any errors
		*/
		ParticlesWaterSystemMaterial(Scene *scene, const int &nb, const std::string name, unsigned int *flag = NULL);

		~ParticlesWaterSystemMaterial();

		void render(Model *model);
		void render(){};

		void print(){};

	private:

		void init();
		//GLuint block_index_lights[4];
		GLuint block_index_camera;
		int nbParticles;
		glm::mat4 M;
};

#endif