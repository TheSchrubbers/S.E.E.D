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
	this->rootObjectNode = new ObjectNode(std::shared_ptr<Scene>(this), "RootObjectNode");
	this->rootLightNode = new Node(std::shared_ptr<Scene>(this), "RootLightNode");
	this->collector = new Collector();
	
	this->cubemap = nullptr;

	this->assimpLoader = new AssimpLoader();

	unsigned int error;
	this->shadowMappingMaterial = new ShadowMappingMaterial(std::shared_ptr<Scene>(this), &error);
	if (error != SEED_SUCCESS)
	{
		delete shadowMappingMaterial;
	}

	this->constructQuad();
	//this->RenderingQuadMaterial = new QuadMaterial(this, "QuadMaterial");

	this->FBObuffer = new FBOBuffer();
	//this->FBObuffer->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
}

Scene::~Scene()
{
	delete rootObjectNode;
	delete collector;
}

ObjectNode* Scene::importModelFromFile(const std::string path, const std::string name)
{
	return this->assimpLoader->importModelFromFile(path, std::shared_ptr<Scene>(this), this->collector, name);
}

void Scene::setCamera(Camera *cam)
{
	this->camera = cam;
}

Camera* Scene::getCamera()
{
	return this->camera;
}

//getters
ObjectNode* Scene::getRootObjectNode()
{
	return this->rootObjectNode;
}

ObjectNode* Scene::getObjectNode(const std::string name)
{
	ObjectNode *n = nullptr;
	//we use an queue to scan the tree of nodes
	std::queue<ObjectNode*> nodes;
	//push the root node
	nodes.push(this->rootObjectNode);
	//we scan all the nodes
	while (!nodes.empty())
	{
		n = nodes.front();
		nodes.pop();
		if (n->getName() == name)
		{
			return n;
		}
		else
		{
			for (int i = 0; i < n->getChildren()->size(); i++)
			{
				nodes.push(n->getChildren()->at(i));
			}
		}
	}
	n = nullptr;
	return nullptr;
}

void Scene::afficher()
{
	std::stack<ObjectNode*> nodes;
	nodes.push(this->rootObjectNode);
	ObjectNode* n = nullptr;
	while (!nodes.empty())
	{
		n = nodes.top();
		nodes.pop();
		std::cout << n->getName() << " : Model : " << (n->getModel() ? 1 : 0) << ", Material : " << (n->getMaterial() ? 1 : 0) << std::endl;
		if (n->getModel())
			n->getModel()->afficher();
		for (int i = 0; i < n->getChildren()->size(); i++)
		{
			nodes.push(n->getChildren()->at(i));
		}
	}
	n = nullptr;
}

void Scene::addPointLight(const glm::vec3 &pos, const glm::vec3 &c, const glm::vec3 &K2, const float &near, const float &far, const float &attenuation, std::string n)
{
	int j = 0;
	std::vector<PointLightNode*> *pointLightNodes = this->collector->getPointLightNodes();
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
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
	node = nullptr;
	pointLightNodes = nullptr;
}

void Scene::addSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &c, const glm::vec3 &K2, int a, const float &attenuation, const float &near, const float &far, std::string n)
{
	int j = 0;
	std::vector<SpotLightNode*> * spotLightNodes = this->collector->getSpotLightNodes();
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
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
	node = nullptr;
	spotLightNodes = nullptr;
}

void Scene::addDirectionnalLight(const glm::vec3 &c, const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &K2, const float &near, const float &far, std::string n)
{
	int j = 0;
	std::vector<DirectionnalLightNode*> *directionnalLightNodes = this->collector->getDirectionnalLightNodes();
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
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
	node = nullptr;
	directionnalLightNodes = nullptr;
}

void Scene::addFlashLight(const glm::vec3 &pos, const glm::vec3 &dir, glm::vec3 &c, const glm::vec3 &K2, const float &near, const float &far, const float &angle, const float &attenuation, std::string n)
{
	int j = 0;
	std::vector<FlashLightNode*> *flashLightNodes = this->collector->getFlashLightNodes();
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
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
	node = nullptr;
	flashLightNodes = nullptr;
}

void Scene::lightsRender()
{
	this->collector->pushPointLights();
	this->collector->pushDirectionnalLights();
	this->collector->pushFlashLights();
	this->collector->pushSpotLights();
}

void Scene::collectRenderedNodes()
{
	this->camera->updateUBO();
	this->collector->collectRenderedObjectNodes(this->rootObjectNode);
	this->lightsRender();
}

Collector* Scene::getCollector()
{
	return this->collector;
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
	for (int i = 0; i < this->collector->getPointLightNodes()->size(); i++)
	{
		pL = this->collector->getPointLightNodes()->at(i)->getLight();
		if (pL->isSendShadow())
		{
			VP = pL->getP() * pL->getV();
			//switching texture
			this->getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			this->FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					this->shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			this->FBObuffer->release();
		}
	}
	for (int i = 0; i < this->collector->getDirectionnalLightNodes()->size(); i++)
	{
		dL = this->collector->getDirectionnalLightNodes()->at(i)->getLight();
		if (dL->isSendShadow())
		{
			VP = dL->getP() * dL->getV();
			//switching texture
			this->getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			this->FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					this->shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			this->FBObuffer->release();
		}
	}
	for (int i = 0; i < this->collector->getSpotLightNodes()->size(); i++)
	{
		sL = this->collector->getSpotLightNodes()->at(i)->getLight();
		if (sL->isSendShadow())
		{
			VP = sL->getP() * sL->getV();
			//switching texture
			this->getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			this->FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					this->shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			this->FBObuffer->release();
		}
	}
	for (int i = 0; i < this->collector->getFlashLightNodes()->size(); i++)
	{
		fL = this->collector->getFlashLightNodes()->at(i)->getLight();
		if (fL->isSendShadow())
		{
			VP = fL->getP() * fL->getV();
			//switching texture
			this->getFBOBuffer()->switchDepthTexture(k);
			//FIRST PASS
			this->FBObuffer->bindWrite();
			glClear(GL_DEPTH_BUFFER_BIT);
			//each node rendering
			for (int j = 0; j < nodes.size(); j++)
			{
				m = nodes[j]->getModel();
				//rendered each node which have model and check shadow mapping
				if (m && nodes[j]->getShadowMapped())
				{
					M = nodes[j]->getMaterial()->getModelMatrix();
					this->shadowMappingMaterial->firstPass(m, M, VP);
				}
			}
			k++;
			this->FBObuffer->release();
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
	this->camera->updateUBO();

	//update each light
	this->lightsRender();

	//Get all the rendered nodes(models, materials...)
	//std::vector<ObjectNode*> *renderedNodes = this->getCollector()->getRenderedCollectedNodes();

	//Shadow mapping rendering
	//this->ShadowMappingRender(nodes);

	//Each node material rendering	
	if (Scene::wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//this->FBObuffer->bindRead(GL_TEXTURE0);
	//each node rendering
	/*for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}*/
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->render();
	}

	//this->FBObuffer->releaseTextures();

	if (this->cubemap)
		this->cubemap->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::SSAOrender()
{
	//update camera
	this->camera->updateUBO();
	//we get all the rendered nodes(models, materials...)
	std::vector<ObjectNode*> *renderedNodes = this->getCollector()->getRenderedCollectedNodes();
	this->lightsRender();

	if (Scene::wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//geometry pass
	/////////////////////////////////////////////////
	//bind FBO
	this->FBObuffer->bindWrite();
	//each node rendering
	for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}
	/////////////////////////////////////////////////

	//light pass
	//release FBO
	this->FBObuffer->release();

	//this->FBObuffer->printTextures();
	
	//deferred lights rendering
	this->RenderingQuadMaterial->render(this->RenderingQuad);

	this->FBObuffer->release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Scene::addNode(ObjectNode* node)
{
	this->rootObjectNode->addChild(node);
	node->setFather((Node*)this->rootObjectNode);
}

bool Scene::setCubeMap(std::string pathDir)
{
	unsigned int flag;
	this->cubemap = new CubeMap(pathDir, std::shared_ptr<Scene>(this), &flag);
	if (flag == SEED_SUCCESS)
	{
		return true;
	}
	return false;
}

CubeMap* Scene::getCubeMap()
{
	return this->cubemap;
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
			this->addNode(n);
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
	this->RenderingQuad = new Model(g, GL_STATIC_DRAW);
	g = nullptr;
}

FBOBuffer* Scene::getFBOBuffer()
{
	return this->FBObuffer;
}

/*void Scene::constructShadowMap()
{

}*/	