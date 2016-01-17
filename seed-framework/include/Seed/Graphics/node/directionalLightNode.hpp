/*
* Copyright(c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty.In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
*including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions :
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software.If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
* /

/*!
* \file DirectionnalLightNode.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef DIRECTIONNALLIGHTNODE_HPP
#define DIRECTIONNALLIGHTNODE_HPP

//OTHER INCLUDES
#include <vector>
#include <iostream>
#include <exception>

//SEED INCLUDES
#include <Seed/Graphics/node/node.hpp>

class DirectionnalLight;
class Scene;

/*! \class DirectionnalLightNode
* \brief DirectionnaltLightNodes who contains Directionnal light nodes
*/
class DirectionnalLightNode : public Node
{
public:

	/*!
	* \brief Constructor of class DirectionnalLightNode
	* \param scene: address of the scene
	* \param name: name of the node
	*/
	DirectionnalLightNode(std::shared_ptr<Scene> scene, const std::string name = "DirectionnalLightNode");
	/*!
	* \brief Destructor of class Node
	*/
	~DirectionnalLightNode();

	//SETTERS & GETTERS
	/*!
	* \brief setting a light to the node
	* \param light: It's the address to the light
	*/
	void setLight(DirectionnalLight *light);
	/*!
	* \brief get the light of the node
	* \return light of the node
	*/
	DirectionnalLight* getLight();
	/*!
	* \brief has light ?
	* \return boolean if the node has light
	*/
	bool hasLight();
	/*!
	* \brief node has children?
	* \return boolean if the node has children
	*/
	bool hasChildren();
	/*!
	* \brief return children
	* \return children
	*/
	std::vector<DirectionnalLightNode*> * getChildren();


private:
	std::vector<DirectionnalLightNode*> m_children;
	DirectionnalLight* directionnalLight;
};

#endif