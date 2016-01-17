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

//OTHER INCLUDES
#include <vector>
#include <iostream>
#include <exception>
#include <memory>

class Scene;
class Model;
class Material;
class Light;

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
		Node(std::shared_ptr<Scene> scene, const std::string name = "node");
		Node(){};
		/*!
		* \brief Destructor of class Node
		*/
		~Node();

		void afficher();

		//SETTERS & GETTERS

		/*!
		* \brief Adding a child's node
		* \param child: Address of the child's node added
		*/
		virtual void addChild(Node* child);
		/*!
		* \brief Set the father's node to the node;
		* \param father: Address of the father's node added
		*/
		virtual void setFather(Node* father);

		/*!
		* \brief get the name of the node
		* \return the name of the node
		*/
		std::string getName();
		/*!
		* \brief get node with its name
		* \param the name of the searched node
		* \return the node
		*/
		virtual Node* getNode(const std::string name);
		/*!
		* \brief is rendering?
		* \return boolean if the node must be rendered
		*/
		bool isRendered();
		/*!
		* \brief node has children?
		* \return boolean if the node has children
		*/
		bool hasChildren();

	private:
		std::vector<Node*> m_children;

	protected:

		std::string name;
		Node* father;
		std::shared_ptr<Scene> scene;
		bool rendered;
};

#endif