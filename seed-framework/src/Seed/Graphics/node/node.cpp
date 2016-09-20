//SEED INCLUDES
#include <Seed/Graphics/node/node.hpp>
#include <Seed/Graphics/model/model.hpp>
//OTHER INCLUDES
#include <queue>
#include <iostream>

Node::Node(std::shared_ptr<Scene> sce, const std::string n)
{
	m_father = NULL;
	m_name = n;
	m_rendered = true;
	m_scene = sce;
	m_isShadowMapped = false;
}

Node::~Node()
{
	for (int i = 0; i < m_children.size(); i++)
		delete m_children[i];
}

void Node::addChild(Node* n)
{
	m_children.push_back(n);
}

void Node::setFather(Node *f)
{
	m_father = f;
}

void Node::afficher()
{}

std::string Node::getName()
{
	return m_name;
}

bool Node::isRendered()
{
	return m_rendered;
}

Node* Node::getNode(const std::string name)
{
	std::queue<Node*> nodes;
	nodes.push(this);
	Node* n;
	while (!nodes.empty())
	{
		n = nodes.front();
		nodes.pop();
		if (n->getName() == name)
			return n;
		for (int i = 0; i < n->m_children.size(); i++)
			nodes.push((Node*)n->m_children[i]);
	}
	return NULL;
}

bool Node::hasChildren()
{
	if (m_children.size())
		return true;
	return false;
}

void Node::setShadowMapped(bool b)
{
	m_isShadowMapped = b;
}

bool Node::getShadowMapped()
{
	return m_isShadowMapped;
}