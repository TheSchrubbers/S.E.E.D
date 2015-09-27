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
* \file node.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <iostream>
#include <exception>

#include <Seed/Graphics/material.hpp>
#include <Seed/Graphics/model.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/light.hpp>

class Scene;
class Model;
class Material;

/*! \class Node
* \brief Nodes who contains differents things of the scene like meshes, lights, cameras...
*/
class Node
{
	public:

		/*!
		* \brief Constructor of class Node
		* \param scene: address of the scene
		* \param name: name of the node
		*/
		Node(Scene *scene, const std::string name = "node");
		/*!
		* \brief Destructor of class Node
		*/
		~Node();
		/*!
		* \brief setting a model to the node
		* \param model: It's the address to the model
		*/
		void setModel(Model *model);
		/*!
		* \brief Adding a child's node
		* \param child: Address of the child's node added
		*/
		void addChild(Node* child);
		/*!
		* \brief Set the father's node to the node;
		* \param father: Address of the father's node added
		*/
		void setFather(Node* father);
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

		void afficher();

		/*!
		* \brief render the node
		*/
		void render();

		/*!
		* \brief get the name of the node
		* \return the name of the node
		*/
		std::string getName();
		/*!
		* \brief get the model of the node
		* \return model of the node
		*/
		Model* getModel();
		/*!
		* \brief get the Mesh of the node
		* \return mesh of the node
		*/
		Material* getMaterial();
		/*!
		* \brief is rendering?
		* \return boolean if the node must be rendered
		*/
		bool isRendered();

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

		std::vector<Node*> m_children;

	private:

		Model *model;
		Material *material;
		std::string name;
		Node* father;
		Light* light;
		Scene* scene;
		bool rendered;
};

#endif