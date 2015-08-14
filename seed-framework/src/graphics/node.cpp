/*!
* \file node.cpp
* \author Jérémy RIFFET
* \version 0.1
*/

#include "Node.hpp"
#include <exception>
#include <iostream>

Node::Node()
{}

Node::~Node()
{}

void Node::addModel(const unsigned int index)
{
	try
	{
		this->m_indexModels.push_back(index);
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "ERROR : Bad alloc memory.\n";
	}
}

void Node::addModel(const unsigned int *index, const unsigned int nb)
{
	try
	{
		for (int i = 0; i < nb; i++)
		{
			this->m_indexModels.push_back((*index));
			index++;
		}
	}
	catch (const std::bad_alloc & e)
	{
		std::cerr << "ERROR : Bad alloc memory.\n";
	}
	catch (const std::exception & e)
	{
		std::cerr << "ERROR : Bad access memory.\n";
	}
}

void Node::addChild(Node* node)
{
	this->m_children.push_back(node);
}


