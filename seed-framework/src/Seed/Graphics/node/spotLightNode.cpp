#include <Seed/Graphics/node/spotLightNode.hpp>

SpotLightNode::SpotLightNode(Scene* sce, const std::string n) : Node(sce, n)
{
	this->spotLight = NULL;
}

SpotLightNode::~SpotLightNode()
{
}

bool SpotLightNode::hasLight()
{
	if (this->spotLight)
		return true;
	return false;
}

SpotLight* SpotLightNode::getLight()
{
	return this->spotLight;
}

std::vector<SpotLightNode*> * SpotLightNode::getChildren()
{
	return &(this->m_children);
}


void SpotLightNode::setLight(SpotLight *l)
{
	this->spotLight = l;
}


