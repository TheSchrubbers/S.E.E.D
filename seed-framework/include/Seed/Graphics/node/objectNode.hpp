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
* \file objectNode.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef OBJECTNODE_HPP
#define OBJECTNODE_HPP

//SEED INCLUDES
#include <Seed/Graphics/node/node.hpp>

class Scene;
class Model;
class Material;

/*! \class ObjectNode
* \brief Nodes who contains differents things of the scene like meshes, cameras...
*/
class ObjectNode : public Node
{
	public:
		/*!
		* \brief Constructor of class ObjectNode
		* \param scene: address of the scene
		* \param name: name of the node
		*/
		ObjectNode(std::shared_ptr<Scene> scene, const std::string name = "objectNode");
		/*!
		* \brief Destructor of class Node
		*/
		~ObjectNode();

		void afficher();
		/*!
		* \brief render the Objectnode
		*/
		void render();

		//SETTERS & GETTERS

		/*!
		* \brief setting a model to the node
		* \param model: It's the address to the model
		*/
		void setModel(Model *model);
		/*!
		* \brief Setting a material to the node
		* \param material: Address of the material
		*/
		void setMaterial(Material* material);

		/*!
		* \brief Setting a material to the node and its children
		* \param material: Address of the material
		*/
		void setMaterialRecur(Material* material);
		/*!
		* \brief get the model of the node
		* \return model of the node
		*/
		Model* getModel();
		/*!
		* \brief get the material of the node
		* \return material of the node
		*/
		Material* getMaterial();
		/*!
		* \brief has material?
		* \return boolean if the node has material
		*/
		bool hasMaterial();
		/*!
		* \brief has model ?
		* \return boolean if the node has model
		*/
		bool hasModel();
		/*!
		* \brief get children
		* \return children
		*/
		std::vector<ObjectNode*>* getChildren();
		/*!
		* \brief Adding a child's node
		* \param child: Address of the child's node added
		*/
		void addChild(ObjectNode* child);
		/*!
		* \brief Set the father's node to the node;
		* \param father: Address of the father's node added
		*/
		void setFather(Node* father);
		/*!
		* \brief get node with its name
		* \param the name of the searched node
		* \return the node
		*/
		ObjectNode* getNode(const std::string name);
		/*!
		* \brief node has children?
		* \return boolean if the node has children
		*/
		bool hasChildren();
	
	private:
		std::vector<ObjectNode*> m_children;
		Model *model;
		Material *material;
};

#endif