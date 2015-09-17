#include <Seed/Graphics/node.hpp>


Node::Node()
{
	this->father = NULL;
	this->model = NULL;
	this->name = "node";
	this->material = NULL;
}

Node::Node(const std::string n)
{
	this->father = NULL;
	this->name = n;
	this->model = NULL;
	this->material = NULL;
}

Node::~Node()
{
	//delete father;
	/*for (int i = 0; i < m_children.size(); i++)
	{
		free(m_children[i]);
	}*/
}

void Node::setModel(Model *m)
{
	this->model = m;
}

void Node::addChild(Node* node)
{
	this->m_children.push_back(node);
}

void Node::setFather(Node *f)
{
	this->father = f;
}

void Node::setMaterial(Material* mat)
{
	this->material = mat;
}

void Node::setMaterialRecur(Material* mat)
{
	std::queue <Node*> nodes;
	nodes.push(this);
	while (!nodes.empty())
	{
		Node *n = nodes.front();
		nodes.pop();
		n->setMaterial(mat);
		for (int i = 0; i < n->m_children.size(); i++)
		{
			nodes.push(n->m_children[i]);
		}
	}
}

void Node::render()
{
	for (int i = 0; i < this->m_children.size(); i++)
	{
		this->m_children[i]->render();
	}

	if (this->material && this->model)
	{
		material->render(this->model);
	}
}

void Node::afficher()
{
	if (this->model)
		this->model->afficher();
}

std::string Node::getName()
{
	return this->name;
}


