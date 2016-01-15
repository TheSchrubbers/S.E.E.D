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
* \file implicitMaterial.hpp
* \brief Material for the implicit surface
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef IMPLICITMATERIAL_HPP
#define IMPLICITMATERIAL_HPP

#include <Seed/Graphics/material.hpp>
#include <Seed/Graphics/marchingCubes/marchingCubes.hpp>

/*! \class ImplicitMaterial
* \brief Material for the explicit surface
*/
class ImplicitMaterial : public Material
{
	public:
		/*!
		* \brief Constructor of class ImplicitMaterial
		* \param scene: address of the scene
		* \param name: name of the material
		* \param reflection: weight of the reflective coefficient
		* \param refraction: weight of the refractive coefficient
		* \param flag: pointer of an int to get any errors
		*/
		ImplicitMaterial(Scene *scene, const std::string name, unsigned int *flag = nullptr, const float reflec = 0.0, const float refrac = 0.0);
		/*!
		* \brief Destructor of the ImplicitMaterial class
		*/
		~ImplicitMaterial();
		/*!
		* \brief Rending implicit surface
		*/
		void render();
		/*!
		* \brief virtual method
		*/
		void print(){};

	private:
		////////////////////////////////
		//Private methods
		////////////////////////////////
		/*!
		* \brief Init implicit material
		*/
		void init();
		////////////////////////////////
		//Attributes
		////////////////////////////////
		GLuint block_index_lights[4];
		GLuint block_index_camera;
		GLuint VBOVertices, VBONormals, VAO;
		glm::mat4 M;
		GLuint edgeTableTex, edgeTable, triTableTex, triTable, dataFieldTex;
		float *dataField;
		std::vector<std::shared_ptr<Point>> pts;
};

#endif