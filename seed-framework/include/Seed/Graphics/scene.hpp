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
* \file scene.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <fstream>
#include <string>
#include <queue>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>

#include <Seed/Graphics/node.hpp>

class Texture;
class Model;
class Material;
class Node;
class Camera;

/*! \class Scene
* \brief class Scene who contains meshes, materials...
*/
class Scene
{
	public:
		/*!
		* \brief Constructor of class Scene
		*/
		Scene();
		/*!
		* \brief Destructor of class Node
		*/
		~Scene();
		/*!
		* \brief Importing a model like obj, 3dmax...
		* \param path: path to the file imported
		* \param name: name of the node
		*/
		bool importModelFromFile(const std::string path, const std::string name);
		/*!
		* \brief Get the rootnode of the scene
		*/
		Node* getRootNode();
		/*!
		* \brief Get camera of the scene
		* \return camera of the scene
		*/
		Camera* getCamera();
		/*!
		* \brief Set camera in the scene
		* \param camera: camera adding in the scene
		*/
		void setCamera(Camera *camera);
		/*!
		* \brief Get the node in the tree of nodes of the scene
		* \param: name of the node
		* \return the node searched
		*/
		Node* getNode(const std::string name);
		/*!
		* \brief get the textures
		* \return textures
		*/
		std::vector<Texture*> getTextures();
		

	private:
		
		Node *rootNode;
		std::vector<Model*> m_models;
		std::vector<Material*> m_materials;
		std::vector<Texture*> m_textures;
		Camera *camera;

		/*!
		* \brief get the number of meshes and materials, build an tree of nodes
		* \param pScene: The assimp scene object
		* \param path : The path to the model file
		*/
		bool loadObjectInScene(const aiScene *pScene, const std::string path, const std::string name);
		/*!
		* \brief Build tree of nodes of the model added
		* \param nodeFather: an ainode object of the tree of nodes
		* \param node: an node object of the tree of nodes
		*/
		void insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, Node *father);
		/*!
		* \brief Load meshes in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		*/
		void loadMeshes(const aiScene *pScene);
		/*!
		* \brief Load materials in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		*/
		void loadMaterials(const aiScene *pScene);
};

#endif