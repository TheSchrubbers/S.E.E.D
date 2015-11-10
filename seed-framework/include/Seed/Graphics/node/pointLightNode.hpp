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
* \file pointLightNode.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef POINTLIGHTNODE_HPP
#define POINTLIGHTNODE_HPP

#include <vector>
#include <iostream>
#include <exception>
#include <Seed/Graphics/node.hpp>


class Scene;
class PointLight;

/*! \class PointLightNode
* \brief PointLightNodes who contains point light nodes
*/
class PointLightNode : public Node
{
	public:

		/*!
		* \brief Constructor of class PointLightNode
		* \param scene: address of the scene
		* \param name: name of the node
		*/
		PointLightNode(Scene *scene, const std::string name = "pointLightNode");
		/*!
		* \brief Destructor of class Node
		*/
		~PointLightNode();

		//SETTERS & GETTERS
		/*!
		* \brief setting a light to the node
		* \param light: It's the address to the light
		*/
		void setLight(PointLight *light);
		/*!
		* \brief get the light of the node
		* \return light of the node
		*/
		PointLight* getLight();
		/*!
		* \brief has light ?
		* \return boolean if the node has light
		*/
		bool hasLight();
		/*!
		* \brief return children
		* \return children		
		*/
		std::vector<PointLightNode*> * getChildren();
		/*!
		* \brief Adding a child's node
		* \param child: Address of the child's node added
		*/
		void addChild(PointLightNode* child);
		/*!
		* \brief Set the father's node to the node;
		* \param father: Address of the father's node added
		*/
		void setFather(Node* father);
		/*!
		* \brief node has children?
		* \return boolean if the node has children
		*/
		bool hasChildren();


	private:
		std::vector<PointLightNode*> m_children;
		PointLight* pointLight;
};

#endif