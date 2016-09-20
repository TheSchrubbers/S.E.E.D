#include <Seed/Graphics/node/pointLightNode.hpp>

PointLightNode::PointLightNode(std::shared_ptr<Scene> sce, const std::string n) : Node(sce, n)
{
	m_pointLight = NULL;
}

PointLightNode::~PointLightNode()
{
}

bool PointLightNode::hasLight()
{
	if (m_pointLight)
		return true;
	return false;
}

void PointLightNode::setLight(PointLight *light)
{
	m_pointLight = light;
}

PointLight* PointLightNode::getLight()
{
	return m_pointLight;
}

std::vector<PointLightNode*> * PointLightNode::getChildren()
{
	return &(m_children);
}

void PointLightNode::addChild(PointLightNode *child)
{
	m_children.push_back(child);
}

void PointLightNode::setFather(Node* f)
{
	m_father = f;
}
bool PointLightNode::hasChildren()
{
	if (m_children.size())
	{
		return true;
	}
	return false;
}




