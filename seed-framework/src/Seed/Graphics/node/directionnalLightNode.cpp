#include <Seed/Graphics/node/directionalLightNode.hpp>

DirectionnalLightNode::DirectionnalLightNode(Scene* sce, const std::string n) : Node(sce, n)
{
	this->directionnalLight = NULL;
}

DirectionnalLightNode::~DirectionnalLightNode()
{
}

bool DirectionnalLightNode::hasLight()
{
	if (this->directionnalLight)
		return true;
	return false;
}

DirectionnalLight* DirectionnalLightNode::getLight()
{
	return this->directionnalLight;
}

std::vector<DirectionnalLightNode*> * DirectionnalLightNode::getChildren()
{
	return &(this->m_children);
}

void DirectionnalLightNode::setLight(DirectionnalLight *l)
{
	this->directionnalLight = l;
}