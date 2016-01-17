//SEED INCLUDES
#include <Seed/Graphics/node/node.hpp>
#include <Seed/Graphics/model/model.hpp>
//OTHER INCLUDES
#include <queue>

Node::Node(std::shared_ptr<Scene> sce, const std::string n)
{
	this->father = NULL;
	this->name = n;
	this->rendered = true;
	this->scene = sce;
}

Node::~Node()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		delete m_children[i];
	}
}


void Node::addChild(Node* n)
{
	this->m_children.push_back(n);
}

void Node::setFather(Node *f)
{
	this->father = f;
}

void Node::afficher()
{}

std::string Node::getName()
{
	return this->name;
}

bool Node::isRendered()
{
	return this->rendered;
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
		{
			return n;
		}
		for (int i = 0; i < n->m_children.size(); i++)
		{
			nodes.push((Node*)n->m_children[i]);
		}
	}
	return NULL;
}

bool Node::hasChildren()
{
	if (this->m_children.size())
	{
		return true;
	}
	return false;
}