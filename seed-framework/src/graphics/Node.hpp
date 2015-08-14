#ifndef NODE_HPP
#define NODE_HPP

#include "Material.hpp"
#include "Model.hpp"
#include <vector>
#include <stdlib.h>

class Node
{
public:
	Node();
	~Node();
	//setters
	void addModel(const unsigned int index);
	void addModel(const unsigned int *index, const unsigned int nb);
	void addChild(Node* node);

private:
	std::vector<int> m_indexModels;
	std::vector<Node*> m_children;
};

#endif