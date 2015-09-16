#include <Seed/Graphics/node.hpp>


Node::Node()
{
	this->father = NULL;
	this->model = NULL;
	this->name = "node";
}

Node::Node(const std::string n)
{
	this->father = NULL;
	this->name = n;
	this->model = NULL;
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

void Node::render()
{
	for (int i = 0; i < this->m_children.size(); i++)
	{
		this->m_children[i]->render();
	}
	if (this->model)
	{
		material->render(this->model);
	}
}

void Node::afficher()
{
	std::cout << "Nom du node : " << this->name << std::endl;
	std::cout << "Pere? : " << (this->father ? 1 : 0) << std::endl;
	std::cout << "Nb de fils : " << this->m_children.size() << std::endl;
	if (this->model)
		this->model->afficher();
}

std::string Node::getName()
{
	return this->name;
}


