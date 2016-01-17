#include <Seed/Graphics/node/directionalLightNode.hpp>

DirectionnalLightNode::DirectionnalLightNode(std::shared_ptr<Scene> sce, const std::string n) : Node(sce, n)
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

bool DirectionnalLightNode::hasChildren()
{
	if (this->m_children.size())
	{
		return true;
	}
	return false;
}