#include <Seed/Graphics/node/pointLightNode.hpp>

PointLightNode::PointLightNode(std::shared_ptr<Scene> sce, const std::string n) : Node(sce, n)
{
	this->pointLight = NULL;
}

PointLightNode::~PointLightNode()
{
}

bool PointLightNode::hasLight()
{
	if (this->pointLight)
		return true;
	return false;
}

void PointLightNode::setLight(PointLight *light)
{
	std::cout << "test" << std::endl;
	this->pointLight = light;
}

PointLight* PointLightNode::getLight()
{
	return this->pointLight;
}

std::vector<PointLightNode*> * PointLightNode::getChildren()
{
	return &(this->m_children);
}

void PointLightNode::addChild(PointLightNode *child)
{
	this->m_children.push_back(child);
}

void PointLightNode::setFather(Node* f)
{
	this->father = f;
}
bool PointLightNode::hasChildren()
{
	if (this->m_children.size())
	{
		return true;
	}
	return false;
}




