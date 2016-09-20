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
* \file SPHMaterial.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef SPHMATERIAL_HPP
#define SPHMATERIAL_HPP

//SEED INCLUDES
#include <Seed/Graphics/engine/material.hpp>

/*! \class SPHMaterial
* \brief SPH Material
*/
class SPHMaterial : public Material
{
	public:

		/*!
		* \brief Constructor of class DefaultMaterial
		* \param scene: address of the scene
		* \param name: name of the material
		* \param flag: pointer of an int to get any errors
		* \param reflection: weight of the reflective coefficient
		* \param refraction: weight of the refractive coefficient
		*/
		SPHMaterial(std::shared_ptr<Scene> scene, const std::string name, unsigned int *flag, const float reflec = 0.0, const float refrac = 0.0);
		~SPHMaterial();

		void render(Model *model);
		void render(){};
		void print(){};

		void gradSurface(std::vector<glm::vec3> pos, glm::vec3 x);

	private:

		void init();
		uniform block_index_lights[4], block_index_camera, NMID, uniformSSBOID;
		SPH *sph;

		std::shared_ptr<Shader> shader;
};

#endif