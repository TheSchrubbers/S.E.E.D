//SEED INCLUDES
#include <Seed/Graphics/engine/scene.hpp>
#include <Seed/Graphics/node/node.hpp>
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/node/pointLightNode.hpp>
#include <Seed/Graphics/node/spotLightNode.hpp>
#include <Seed/Graphics/node/directionalLightNode.hpp>
#include <Seed/Graphics/node/flashLightNode.hpp>
#include <Seed/Graphics/lights/directionnalLight.hpp>
#include <Seed/Graphics/lights/flashLight.hpp>
#include <Seed/Graphics/lights/pointLight.hpp>
#include <Seed/Graphics/lights/spotLight.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/cubeMap.hpp>
#include <Seed/Graphics/materials/ParticlesSystemMaterial/ParticlesWaterSystemMaterial/ParticlesWaterSystemMaterial.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/materials/SSAOMaterial/QuadMaterial/QuadMaterial.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>
#include <Seed/Graphics/model/geometry.hpp>
#include <Seed/Graphics/loaders/assimpLoader/assimpLoader.hpp>
#include <Seed/Graphics/materials/ShadowMappingMaterial/ShadowMappingMaterial.hpp>


//STATIC ATTRIBUTS
bool Scene::wireframe = false;
bool Scene::normalMapActive = true;
bool Scene::normalMapView = false;
bool Scene::parallaxMapActive = true;
bool Scene::parallaxMapView = false;
bool Scene::specularMapActive = true;
bool Scene::specularMapView = false;
bool Scene::shadowMapActive = true;
float Scene::biasParallax = 0.5;
float Scene::bias = 0.0005;

Scene::Scene()
{
    initializeOpenGLFunctions();
	
	m_rootObjectNode = new ObjectNode(std::shared_ptr<Scene>(this), "RootObjectNode");
	m_rootLightNode = new Node(std::shared_ptr<Scene>(this), "RootLightNode");
	m_collector = new Collector();
	
	m_cubemap = nullptr;

	m_assimpLoader = new AssimpLoader();

	/*unsigned int error;
	m_shadowMappingMaterial = new ShadowMappingMaterial(std::shared_ptr<Scene>(this), &error);
	if (error != SEED_SUCCESS)
		delete m_shadowMappingMaterial;*/

	/*this->constructQuad();
	//m_RenderingQuadMaterial = new QuadMaterial(this, "QuadMaterial");*/

	m_FBObuffer = new FBOBuffer();
	//m_FBObuffer->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
}

Scene::~Scene()
{
	delete m_rootObjectNode;
	delete m_collector;
}

ObjectNode* Scene::importModelFromFile(const std::string path, const std::string name)
{
	return m_assimpLoader->importModelFromFile(path, std::shared_ptr<Scene>(this), m_collector, name);
}

void Scene::setCamera(Camera *cam)
{
	m_camera = cam;
}

Camera* Scene::getCamera()
{
	return m_camera;
}

//getters
ObjectNode* Scene::getRootObjectNode()
{
	return m_rootObjectNode;
}

ObjectNode* Scene::getObjectNode(const std::string name)
{
	ObjectNode *n = nullptr;
	//we use an queue to scan the tree of nodes
	std::queue<ObjectNode*> nodes;
	//push the root node
	nodes.push(m_rootObjectNode);
	//we scan all the nodes
	while (!nodes.empty())
	{
		n = nodes.front();
		nodes.pop();
		if (n->getName() == name)
			return n;
		else
			for (int i = 0; i < n->getChildren()->size(); i++)
				nodes.push(n->getChildren()->at(i));
	}
	n = nullptr;
	return nullptr;
}

void Scene::afficher()
{
	std::stack<ObjectNode*> nodes;
	nodes.push(m_rootObjectNode);
	ObjectNode* n = nullptr;
	while (!nodes.empty())
	{
		n = nodes.top();
		nodes.pop();
		std::cout << n->getName() << " : Model : " << (n->getModel() ? 1 : 0) << ", Material : " << (n->getMaterial() ? 1 : 0) << std::endl;
		if (n->getModel())
			n->getModel()->afficher();
		for (int i = 0; i < n->getChildren()->size(); i++)
			nodes.push(n->getChildren()->at(i));
	}
	n = nullptr;
}

void Scene::addPointLight(const glm::vec3 &pos, const glm::vec3 &c, const glm::vec3 &K2, const float &near, const float &far, const float &attenuation, std::string n)
{
	int j = 0;
	std::vector<PointLightNode*> *pointLightNodes = m_collector->getPointLightNodes();
	PointLightNode *node = nullptr;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < pointLightNodes->size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == pointLightNodes->at(i)->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}	
	//create a new node and push it like the children of node lights
	node = new PointLightNode  (std::shared_ptr<Scene>(this), n);	
	pointLightNodes->push_back(node);	
	node->setLight(new PointLight(n, pos, K2, c, near, far, attenuation));
	m_rootLightNode->addChild((Node*)node);
	node->setFather(m_rootLightNode);
	node = nullptr;
	pointLightNodes = nullptr;
}

void Scene::addSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &c, const glm::vec3 &K2, int a, const float &attenuation, const float &near, const float &far, std::string n)
{
	int j = 0;
	std::vector<SpotLightNode*> * spotLightNodes = m_collector->getSpotLightNodes();
	SpotLightNode *node = nullptr;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < spotLightNodes->size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == spotLightNodes->at(i)->getName())
		{
			n += ("_" + std::to_string(j));  
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	node = new SpotLightNode(std::shared_ptr<Scene>(this), n);
	spotLightNodes->push_back(node);
	node->setLight(new SpotLight(n, pos, dir, K2, c, a, near, far, attenuation));
	m_rootLightNode->addChild((Node*)node);
	node->setFather(m_rootLightNode);
	node = nullptr;
	spotLightNodes = nullptr;
}

void Scene::addDirectionnalLight(const glm::vec3 &c, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &K2, const float &near, const float &far, std::string n)
{
	int j = 0;
	std::vector<DirectionnalLightNode*> *directionnalLightNodes = m_collector->getDirectionnalLightNodes();
	DirectionnalLightNode *node = nullptr;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < directionnalLightNodes->size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == directionnalLightNodes->at(i)->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	node = new DirectionnalLightNode(std::shared_ptr<Scene>(this), n);
	directionnalLightNodes->push_back(node);
	node->setLight(new DirectionnalLight(n, pos, dir, K2, c, near, far));
	m_rootLightNode->addChild((Node*)node);
	node->setFather(m_rootLightNode);
	node = nullptr;
	directionnalLightNodes = nullptr;
}

void Scene::addFlashLight(const glm::vec3 &pos, const glm::vec3 &dir, glm::vec3 &c, const glm::vec3 &K2, const float &near, const float &far, const float &angle, const float &attenuation, std::string n)
{
	int j = 0;
	std::vector<FlashLightNode*> *flashLightNodes = m_collector->getFlashLightNodes();
	FlashLightNode *node = nullptr;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < flashLightNodes->size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == flashLightNodes->at(i)->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	node = new FlashLightNode(std::shared_ptr<Scene>(this), n);
	flashLightNodes->push_back(node);
	node->setLight(new FlashLight(n, pos, dir, K2, c, angle, near, far, attenuation));
	m_rootLightNode->addChild((Node*)node);
	node->setFather(m_rootLightNode);
	node = nullptr;
	flashLightNodes = nullptr;
}

void Scene::lightsRender()
{
	m_collector->pushPointLights();
	m_collector->pushDirectionnalLights();
	m_collector->pushFlashLights();
	m_collector->pushSpotLights();
}

void Scene::collectRenderedNodes()
{
	m_camera->updateUBO();
	m_collector->collectRenderedObjectNodes(m_rootObjectNode);
	lightsRender();
}

Collector* Scene::getCollector()
{
	return m_collector;
}

void Scene::ShadowMappingRender(std::vector<ObjectNode*> nodes)
{
	glm::mat4 M;
	Model* m = nullptr;
	PointLight *pL = nullptr;
	SpotLight *sL = nullptr;
	FlashLight *fL = nullptr;
	DirectionnalLight *dL = nullptr;
	glm::mat4 VP;
	int k = 0;
	for (int i = 0; i < m_collector->getPointLightNodes()->size(); i++)
	{
		pL = m_collector->getPointLightNodes()->at(i)->getLight();
		if (pL->isSendShadow())
		{
			VP = pL->getP() * pL->getV();
			//switching texture
			getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			m_FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					m_shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			m_FBObuffer->release();
		}
	}
	for (int i = 0; i < m_collector->getDirectionnalLightNodes()->size(); i++)
	{
		dL = m_collector->getDirectionnalLightNodes()->at(i)->getLight();
		if (dL->isSendShadow())
		{
			VP = dL->getP() * dL->getV();
			//switching texture
			getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			m_FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					m_shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			m_FBObuffer->release();
		}
	}
	for (int i = 0; i < m_collector->getSpotLightNodes()->size(); i++)
	{
		sL = m_collector->getSpotLightNodes()->at(i)->getLight();
		if (sL->isSendShadow())
		{
			VP = sL->getP() * sL->getV();
			//switching texture
			getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			m_FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					m_shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			m_FBObuffer->release();
		}
	}
	for (int i = 0; i < m_collector->getFlashLightNodes()->size(); i++)
	{
		fL = m_collector->getFlashLightNodes()->at(i)->getLight();
		if (fL->isSendShadow())
		{
			VP = fL->getP() * fL->getV();
			//switching texture
			getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			m_FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					m_shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			m_FBObuffer->release();
		}
	}
	m = nullptr;
	pL = nullptr;
	sL = nullptr;
	fL = nullptr;
	dL = nullptr;
}

void Scene::render(std::vector<ObjectNode*> nodes)
{
	//update camera
	m_camera->updateUBO();

	//update each light
	//lightsRender();

	//Get all the rendered nodes(models, materials...)
	//std::vector<ObjectNode*> *renderedNodes = this->getCollector()->getRenderedCollectedNodes();

	//Shadow mapping rendering
	//this->ShadowMappingRender(nodes);

	//Each node material rendering	
	//if (Scene::wireframe)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_FBObuffer->bindRead(GL_TEXTURE0);
	//each node rendering
	/*for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}*/
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->render();

	//m_FBObuffer->releaseTextures();
	//std::cout << m_cubemap << std::endl;
	if (m_cubemap)
		m_cubemap->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::SSAOrender()
{
	//update camera
	m_camera->updateUBO();
	//we get all the rendered nodes(models, materials...)
	std::vector<ObjectNode*> *renderedNodes = getCollector()->getRenderedCollectedNodes();
	lightsRender();

	if (Scene::wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//geometry pass
	/////////////////////////////////////////////////
	//bind FBO
	m_FBObuffer->bindWrite();
	//each node rendering
	for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}
	/////////////////////////////////////////////////

	//light pass
	//release FBO
	m_FBObuffer->release();

	//m_FBObuffer->printTextures();
	
	//deferred lights rendering
	m_RenderingQuadMaterial->render(m_RenderingQuad);

	m_FBObuffer->release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::addNode(ObjectNode* node)
{
	m_rootObjectNode->addChild(node);
	node->setFather((Node*)m_rootObjectNode);
}

bool Scene::setCubeMap(std::string pathDir)
{
	unsigned int flag;
	m_cubemap = new CubeMap(pathDir, std::shared_ptr<Scene>(this), &flag);
	if (flag == SEED_SUCCESS)
	{
		return true;
	}
	return false;
}

CubeMap* Scene::getCubeMap()
{
	return m_cubemap;
}

void Scene::addWaterSystemParticles(const glm::vec3 &positionStarter, const int &typeShape, const int &nb, unsigned int *flag, std::string name)
{
	GLfloat square[12];
	GLuint indices[6];
	Geometry *g = nullptr;
	InstancedModel *mo = nullptr;
	ObjectNode *n = new ObjectNode(std::shared_ptr<Scene>(this), name);
	ParticlesWaterSystemMaterial *m = new ParticlesWaterSystemMaterial(std::shared_ptr<Scene>(this), nb, name + "_Material", flag);
	if (flag)
	{
		if (*flag == SEED_SUCCESS)
		{
			n->setMaterial(m);
			g = new Geometry();
			square[0] = 0.0f; square[1] = 1.0f; square[2] = 0.0f;
			square[3] = 1.0f; square[4] = 1.0f; square[5] = 0.0f;
			square[6] = 1.0f; square[7] = 0.0f; square[8] = 0.0f;
			square[9] = 0.0f; square[10] = 0.0f; square[11] = 0.0f;
			indices[0] = 0; indices[1] = 1; indices[2] = 2;
			indices[3] = 0; indices[4] = 2; indices[5] = 3;
			g->setVertices(square, 12, 3);
			g->setFaces(GL_TRIANGLES, indices, 6);
			mo = new InstancedModel(g);
			n->setModel(mo);
			addNode(n);
		}
	}
		g = nullptr;
		mo = nullptr;
		n = nullptr;
		m = nullptr;
}

void Scene::constructQuad()
{
	float vertices[] = {
		-1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
		1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		-1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0
	};
	Geometry *g = new Geometry();
	g->setVertices(vertices, 18, 3);
	m_RenderingQuad = new Model(g, GL_STATIC_DRAW);
	g = nullptr;
}

FBOBuffer* Scene::getFBOBuffer()
{
	return m_FBObuffer;
}

/*void Scene::constructShadowMap()
{

}*/	