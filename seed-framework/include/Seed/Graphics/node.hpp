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

/*! \class Node
* \brief Nodes who contains differents things of the scene like meshes, lights, cameras...
*/
class Node
{
	public:
		/*!
		* \brief Constructor of class Node
		*/
		Node();
		/*!
		* \brief Constructor of class Node
		* \param name: name of the node
		*/
		Node(const std::string name);
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

		std::vector<Node*> m_children;

	private:
		Model *model;
		Material *material;
		std::string name;
		Node* father;
};

#endif