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
* \file material.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <GL/glew.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <Seed/Graphics/texture.hpp>
#include <Seed/Graphics/model.hpp>

class Camera;
class Model;
/*! \class Material
* \brief Material of a node
*/
class Material
{
	public:

		/*!
		* \brief Constructor of class Material
		* \param material: address of the aimaterial
		* \param camera: address of the camera in the scene
		*/
		Material(const aiMaterial *material, Camera *camera);
		/*!
		* \brief Constructor of class Material
		* \param camera: address of the camera in the scene
		*/
		Material(Camera *camera);

		~Material(){};

		/*!
		* \brief Load shaders from the directory "Shaders" in directory of this material
		* \param pathDir: path to the directory of the shaders
		*/
		void addShaders(const std::string pathDir);

		void render(Model *model);
		/*!
		* \brief Push texture in the textures' array
		* \param t: Address of texture to push
		*/
		void pushTexture(Texture *texture);

	private:

		GLuint programID;
		std::vector<Texture*> textures;
		Camera *camera;
};

#endif