#ifndef ROOTNODE_HPP
#define ROOTNODE_HPP

#include "Node.hpp"
#include <vector>
#include <stdlib.h>

class RootNode
{
public:
	RootNode();
	~RootNode();
	Node* getNode(const int index);
	Node* getNode(const char *nom);
	bool createNode(const char* nom);
private:
	std::vector<Node> m_children;
};

#endif