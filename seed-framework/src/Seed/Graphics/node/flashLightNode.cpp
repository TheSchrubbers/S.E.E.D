#include <Seed/Graphics/node/flashLightNode.hpp>

FlashLightNode::FlashLightNode(std::shared_ptr<Scene> sce, const std::string n) : Node(sce, n)
{
	this->flashLight = NULL;
}

FlashLightNode::~FlashLightNode()
{
}

bool FlashLightNode::hasLight()
{
	if (this->flashLight)
		return true;
	return false;
}

FlashLight* FlashLightNode::getLight()
{
	return this->flashLight;
}

std::vector<FlashLightNode*> * FlashLightNode::getChildren()
{
	return &(this->m_children);
}

void FlashLightNode::setLight(FlashLight *l)
{
	this->flashLight = l;
}

bool FlashLightNode::hasChildren()
{
	if (this->m_children.size())
	{
		return true;
	}
	return false;
}



