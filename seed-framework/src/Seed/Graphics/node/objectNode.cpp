//SEED INCLUDES
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/engine/material.hpp>
#include <Seed/Graphics/model/model.hpp>
#include <Seed/Graphics/engine/scene.hpp>
#include <Seed/Graphics/materials/ShadowMappingMaterial/ShadowMappingMaterial.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>
//OTHER INCLUDES
#include <vector>
#include <iostream>
#include <exception>

ObjectNode::ObjectNode(std::shared_ptr<Scene> sce, const std::string n) : Node(sce, n)
{
	m_model = nullptr;
	m_material = nullptr;
}

ObjectNode::~ObjectNode()
{
	for (int i = 0; i < m_children.size(); i++)
		if (m_children[i])
			delete m_children[i];
}

void ObjectNode::setModel(Model *m)
{
	m_model = m;
}

void ObjectNode::setMaterial(Material* mat)
{
	m_material = mat;
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
			ObjectNodes.push(n->m_children[i]);
	}
}

void ObjectNode::render()
{
	if (m_rendered)
	{
		int size = m_children.size();
		for (int i = 0; i < size; i++)
			m_children[i]->render();
		if (m_material)
			if (m_model)
				m_material->render(m_model);
			else
				m_material->render();
	}
}

void ObjectNode::afficher()
{
}

Model* ObjectNode::getModel()
{
	return m_model;
}

Material* ObjectNode::getMaterial()
{
	return m_material;
}

bool ObjectNode::hasMaterial()
{
	if (m_material)
		return true;
	return false;
}

bool ObjectNode::hasModel()
{
	if (m_model)
		return true;
	return false;
}

std::vector<ObjectNode*>* ObjectNode::getChildren()
{
	return &(m_children);
}

void ObjectNode::addChild(ObjectNode* n)
{
	m_children.push_back(n);
}

void ObjectNode::setFather(Node *f)
{
	m_father = f;
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
			return n;
		for (int i = 0; i < n->m_children.size(); i++)
			nodes.push((ObjectNode*)n->m_children[i]);
	}
	return NULL;
}

bool ObjectNode::hasChildren()
{
	if (m_children.size())
		return true;
	return false;
}