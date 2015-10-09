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
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>

class Scene;
/*! \class Material
* \brief Material of a node
*/
class Material
{
	public:

		/*!
		* \brief Constructor of class Material
		* \param material: address of the aimaterial
		* \param scene: address of the scene
		* \param name: name of the material
		*/
		Material(const aiMaterial *material, Scene *scene, std::string name, unsigned int *flag = NULL);
		/*!
		* \brief Constructor of class Material
		* \param scene: address of the scene
		* \param name: name of the material
		* \param pathShaders: path to the directory who contains shaders' files
		*/
		Material(Scene *scene, const std::string name, const std::string pathShaders = "", unsigned int *flag = NULL);

		~Material();

		/*!
		* \brief Load shaders from the directory "Shaders" in directory of this material
		* \param pathDir: path to the directory of the shaders
		*/
		bool addShaders(const std::string pathDir);

		virtual void render(Model *model) = 0;
		/*!
		* \brief Push texture in the textures' array
		* \param t: Address of texture to push
		*/
		void pushTexture(Texture *texture);
		/*!
		* \brief adding texture to the material since the textur es' folder
		* \param path to the texture
		* \param scene: address of the current scene
		* \return true if the texture is adding
		*/
		void addTexture(const std::string pathTexture, Scene *scene, unsigned int type, unsigned int *flag = NULL);
		/*!
		* \brief setting composants of material
		* \param a: ambiant composant
		* \param d: diffuse composant
		* \param s: specular composant
		*/
		void setLight(float a, float d, float s);
		/*!
		* \brief Transform the model matrix
		* \param T: Vector of translation
		*/
		void translate(glm::vec3 T);
		/*!
		* \brief Active textures
		*/
		void activeTextures();
		/*!
		* \brief Release textures
		*/
		void releaseTextures();

		/*!
		* \brief Print textures
		*/
		void printTextures();

	protected:

		GLuint programID;
		std::vector<Texture*> textures_ambiant;
		std::vector<Texture*> textures_diffuse;
		std::vector<Texture*> textures_specular;
		Scene *scene;
		Camera *camera;
		std::string name;

		struct compLight
		{
			float ambiant, diffuse, specular;
			GLuint ambiantID, diffuseID, specularID;
		};

		glm::mat4 M, Normal_Matrix;
		GLuint MID, NMID;
		compLight compl;
};

#endif