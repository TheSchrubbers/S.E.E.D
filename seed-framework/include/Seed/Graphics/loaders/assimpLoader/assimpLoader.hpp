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
* \file assimpLoader.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef ASSIMPLOADER_HPP
#define ASSIMPLOADER_HPP

#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <memory>

class Collector;
class ObjectNode;
class Scene;

/*! \class AssimpLoader
* \brief class AssimpLoader, loader models
*/
class AssimpLoader
{
	public:
		/*!
		* \brief Constructor of class AssimpLoader
		*/
		AssimpLoader();
		/*!
		* \brief Destructor of class Node
		*/
		~AssimpLoader();
		/*!
		* \brief Importing a model like obj, 3dmax...
		* \param path: path to the file imported
		* \param name: name of the node
		*/
		ObjectNode* importModelFromFile(const std::string path, std::shared_ptr<Scene> scene, Collector *collector, const std::string name);
	private:

		/*!
		* \brief get the number of meshes and materials, build an tree of nodes
		* \param pScene: The assimp scene object
		* \param path : The path to the model file
		*/
		ObjectNode* loadObjectInScene(const aiScene *pScene, const std::string path, std::shared_ptr<Scene> scene, Collector *collector, const std::string name);
		/*!
		* \brief Build tree of nodes of the model added
		* \param nodeFather: an ainode object of the tree of nodes
		* \param node: an objectNode object of the tree of objectnodes
		*/
		void insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, ObjectNode *father, std::shared_ptr<Scene> scene, Collector *collector);
		/*! 
		* \brief Load meshes in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		* \param path: path name of the mesh importing
		*/
		void loadMeshes(const aiScene *pScene, Collector *collector, std::string path);
		/*!
		* \brief Load materials in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		* \param name: name of the material
		*/
		//void loadMaterials(const aiScene *pScene, std::string name);
};

#endif