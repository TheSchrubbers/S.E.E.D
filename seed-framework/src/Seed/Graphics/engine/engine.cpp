//SEED INCLUDES
#include "Seed/Graphics/engine/engine.hpp"

using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::stack;

Engine::Engine()
{
	initLog();
}

Engine::~Engine()
{
}

shared_ptr<Scene> Engine::createScene()
{
    initializeOpenGLFunctions();
	//create scene
	m_scene = std::make_shared<Scene>();
	return m_scene;
}

vector<ObjectNode*> Engine::loadSystemToRendering()
{
	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;
	vector<PointLightNode*> *pointLightNodes;
	vector<FlashLightNode*> *flashLightNodes;
	vector<DirectionnalLightNode*> *  directionnalLightNodes;
	vector<SpotLightNode*> * spotLightNodes;
	stack<ObjectNode*> n;
	vector<ObjectNode*> nodes;
	ObjectNode *n2 = nullptr;


	//get all the nodes with a model
	n.push(m_scene->getRootObjectNode());
	while (!n.empty())
	{
		n2 = n.top();
		n.pop();
		for (int i = 0; i < n2->getChildren()->size(); i++)
			n.push(n2->getChildren()->at(i));
		if (n2->getModel())
			nodes.push_back(n2);
	}

	//set FBO buffer textures for shadow mapping
	pointLightNodes = m_scene->getCollector()->getPointLightNodes();
	for (int i = 0; i < pointLightNodes->size(); i++)
		if (pointLightNodes->at(i)->getLight()->isSendShadow())
			m_scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	directionnalLightNodes = m_scene->getCollector()->getDirectionnalLightNodes();
	for (int i = 0; i < directionnalLightNodes->size(); i++)
		if (directionnalLightNodes->at(i)->getLight()->isSendShadow())
			m_scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	spotLightNodes = m_scene->getCollector()->getSpotLightNodes();
	for (int i = 0; i < spotLightNodes->size(); i++)
		if (spotLightNodes->at(i)->getLight()->isSendShadow())
			m_scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	flashLightNodes = m_scene->getCollector()->getFlashLightNodes();
	for (int i = 0; i < flashLightNodes->size(); i++)
		if (flashLightNodes->at(i)->getLight()->isSendShadow())
			m_scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

	pointLightNodes = nullptr;
	spotLightNodes = nullptr;
	directionnalLightNodes = nullptr;
	flashLightNodes = nullptr;
	n2 = nullptr;

	m_nodes = nodes;

	return nodes;
}
void Engine::mainRender()
{
	// main loop to render
		
		//update mouse control and keyboard control
		//m_controller->updateControl(this->window, scene->getCamera(), deltaTime);

	m_scene->render(m_nodes);

	// Clear the depthbuffer and the colourbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//time between 2 frames
		//deltaTime = float(lastTime - currentTime);
}

bool Engine::initController()
{
	//initialize Controller to update viewing camera
	m_controller = new Controller();
	return true;
}

void Engine::updateDragMouseControl(float dragPosX, float dragPosY)
{
	m_controller->updateDragMouseControl(m_scene->getCamera(), dragPosX, dragPosY);
}

shared_ptr<Scene> Engine::getScene()
{
	return m_scene;
}

Controller* Engine::getController()
{
	return m_controller;
}