#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/material.hpp>
#include <Seed/Graphics/model.hpp>
#include <Seed/Graphics/scene.hpp>
#include <vector>
#include <iostream>
#include <exception>

ObjectNode::ObjectNode(Scene* sce, const std::string n) : Node(sce, n)
{
	this->model = NULL;
	this->material = NULL;
}

ObjectNode::~ObjectNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
	}
}

void ObjectNode::setModel(Model *m)
{
	this->model = m;
}

void ObjectNode::setMaterial(Material* mat)
{
	this->material = mat;
}

void ObjectNode::setMaterialRecur(Material* mat)
{
	std::queue <ObjectNode*> ObjectNodes;
	ObjectNodes.push(this);
	while (!ObjectNodes.empty())
	{
		ObjectNode *n = ObjectNodes.front();
		ObjectNodes.pop();
		n->setMaterial(mat);
		for (int i = 0; i < n->m_children.size(); i++)
		{
			ObjectNodes.push(n->m_children[i]);
		}
	}
}

void ObjectNode::render()
{
	if (this->rendered)
	{
		for (int i = 0; i < this->m_children.size(); i++)
		{
			this->m_children[i]->render();
		}

		if (this->material)
		{
			if (this->model)
				material->render(this->model);
			/*else
				material->render();*/
		}
	}
}

void ObjectNode::afficher()
{
}

Model* ObjectNode::getModel()
{
	return this->model;
}

Material* ObjectNode::getMaterial()
{
	return this->material;
}

bool ObjectNode::hasMaterial()
{
	if (this->material)
		return true;
	return false;
}

bool ObjectNode::hasModel()
{
	if (this->model)
		return true;
	return false;
}

std::vector<ObjectNode*>* ObjectNode::getChildren()
{
	return &(this->m_children);
}

void ObjectNode::addChild(ObjectNode* n)
{
	this->m_children.push_back(n);
}

void ObjectNode::setFather(Node *f)
{
	this->father = f;
}

ObjectNode* ObjectNode::getNode(const std::string name)
{
	std::queue<ObjectNode*> nodes;
	nodes.push(this);
	ObjectNode* n;
	while (!nodes.empty())
	{
		n = nodes.front();
		nodes.pop();
		if (n->getName() == name)
		{
			return n;
		}
		for (int i = 0; i < n->m_children.size(); i++)
		{
			nodes.push((ObjectNode*)n->m_children[i]);
		}
	}
	return NULL;
}