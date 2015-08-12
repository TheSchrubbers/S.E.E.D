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
private:
	std::vector<int> m_indexModels;
	std::vector<Node> m_children;
};

#endif