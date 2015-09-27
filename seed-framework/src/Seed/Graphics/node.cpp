#include <Seed/Graphics/node.hpp>

Node::Node(Scene* sce, const std::string n)
{
	this->father = NULL;
	this->name = n;
	this->model = NULL;
	this->material = NULL;
	this->light = NULL;
	this->rendered = true;
	this->scene = sce;
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

void Node::setLight(Light *l)
{
	this->light = l;
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
	if (this->rendered)
	{
		for (int i = 0; i < this->m_children.size(); i++)
		{
			//std::cout << this->getName() << " : " << (this->getModel() ? 1 : 0) << ", " << (this->getMaterial() ? 1 : 0);
			this->m_children[i]->render();
		}

		if (this->material)
		{
			if (this->model)
				material->render(this->model);
			/*else
				material->render();*/
		}
		else if(this->light)
		{
			this->light->render();
		}
	}
}

void Node::afficher()
{
}

std::string Node::getName()
{
	return this->name;
}

Model* Node::getModel()
{
	return this->model;
}

Material* Node::getMaterial()
{
	return this->material;
}

bool Node::isRendered()
{
	return this->rendered;
}

bool Node::hasMaterial()
{
	if (this->material)
		return true;
	return false;
}

bool Node::hasModel()
{
	if (this->model)
		return true;
	return false;
}

bool Node::hasLight()
{
	if (this->light)
		return true;
	return false;
}

Light* Node::getLight()
{
	return this->light;
}



