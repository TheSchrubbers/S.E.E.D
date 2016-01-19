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
bool Scene::normalMappingActive = true;
bool Scene::specularMapActive = true;
bool Scene::specularMapView = false;

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
	this->FBObuffer->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
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
	//we use an queue to scan the tree of nodes
	std::queue<ObjectNode*> nodes;
	//push the root node
	nodes.push(this->rootObjectNode);
	//we scan all the nodes
	while (!nodes.empty())
	{
		ObjectNode *n = nodes.front();
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
	return nullptr;
}

void Scene::afficher()
{
	std::stack<ObjectNode*> nodes;
	nodes.push(this->rootObjectNode);
	while (!nodes.empty())
	{
		ObjectNode* n = nodes.top();
		nodes.pop();
		std::cout << n->getName() << " : Model : " << (n->getModel() ? 1 : 0) << ", Material : " << (n->getMaterial() ? 1 : 0) << std::endl;
		if (n->getModel())
			n->getModel()->afficher();
		for (int i = 0; i < n->getChildren()->size(); i++)
		{
			nodes.push(n->getChildren()->at(i));
		}
	}
}

void Scene::addPointLight(const glm::vec3 &pos, const glm::vec3 &c, const glm::vec3 &K2, int dist, std::string n)
{
	int j = 0;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < this->collector->m_pointLightNodes.size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == this->collector->m_pointLightNodes[i]->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	PointLightNode *node = new PointLightNode(std::shared_ptr<Scene>(this), n);
	this->collector->m_pointLightNodes.push_back(node);
	node->setLight(new PointLight(n, pos, K2, c, dist));
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
}

void Scene::addSpotLight(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &c, const glm::vec3 &K2, int a, int dist, std::string n)
{
	int j = 0;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < this->collector->m_spotLightNodes.size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == this->collector->m_spotLightNodes[i]->getName())
		{
			n += ("_" + std::to_string(j));  
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	SpotLightNode *node = new SpotLightNode(std::shared_ptr<Scene>(this), n);
	this->collector->m_spotLightNodes.push_back(node);
	node->setLight(new SpotLight(n, pos, dir, K2, c, a, dist));
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
}

void Scene::addDirectionnalLight(const glm::vec3 &c, const glm::vec3 &dir, const glm::vec3 &K2, std::string n)
{
	int j = 0;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < this->collector->m_directionnalLightNodes.size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == this->collector->m_directionnalLightNodes[i]->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	DirectionnalLightNode *node = new DirectionnalLightNode(std::shared_ptr<Scene>(this), n);
	this->collector->m_directionnalLightNodes.push_back(node);
	node->setLight(new DirectionnalLight(n, dir, K2, c));
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
}

void Scene::addFlashLight(const glm::vec3 &pos, const glm::vec3 &dir, glm::vec3 &c, const glm::vec3 &K2, int dist, std::string n)
{
	int j = 0;
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < this->collector->m_flashLightNodes.size(); i++)
	{
		j++;
		//if it's true we modify the given name
		if (n == this->collector->m_flashLightNodes[i]->getName())
		{
			n += ("_" + std::to_string(j));
			i = 0;
		}
	}
	//create a new node and push it like the children of node lights
	FlashLightNode *node = new FlashLightNode(std::shared_ptr<Scene>(this), n);
	this->collector->m_flashLightNodes.push_back(node);
	node->setLight(new FlashLight(n, pos, dir, K2, c, dist));
	this->rootLightNode->addChild((Node*)node);
	node->setFather(this->rootLightNode);
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
	//FIRST PASS
	this->FBObuffer->bindWrite();
	glClear(GL_DEPTH_BUFFER_BIT);
	//each node rendering
	for (int i = 0; i < nodes.size(); i++)
	{
		Model* m = nodes[i]->getModel();
		//rendered each node which have model and check shadow mapping
		if (m && nodes[i]->getShadowMapped())
		{
			this->shadowMappingMaterial->firstPass(m);
		}
	}
	this->FBObuffer->release();

	//SECOND PASS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->FBObuffer->bindRead(GL_TEXTURE0);
	//each node rendering
	/*for (int i = 0; i < nodes.size(); i++)
	{
		Model* m = nodes[i]->getModel();
		//rendered each node which have model and check shadow mapping
		if (m && nodes[i]->getShadowMapped())
		{
			this->shadowMappingMaterial->secondPass(m);
		}
	}*/
	this->shadowMappingMaterial->secondPass(this->RenderingQuad);
	this->FBObuffer->releaseTextures();

}

void Scene::render(std::vector<ObjectNode*> nodes)
{
	//update camera
	this->camera->updateUBO();

	//update each light
	this->lightsRender();

	//Get all the rendered nodes(models, materials...)
	std::vector<ObjectNode*> *renderedNodes = this->getCollector()->getRenderedCollectedNodes();

	//Shadow mapping rendering
	//this->ShadowMappingRender(nodes);

	//Each node material rendering	
	if (Scene::wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//each node rendering
	for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}

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
	ObjectNode *n = new ObjectNode(std::shared_ptr<Scene>(this), name);
	ParticlesWaterSystemMaterial *m = new ParticlesWaterSystemMaterial(std::shared_ptr<Scene>(this), nb, name + "_Material", flag);
	if (flag)
	{
		if (*flag == SEED_SUCCESS)
		{
			n->setMaterial(m);
			Geometry *g = new Geometry();
			square[0] = 0.0f; square[1] = 1.0f; square[2] = 0.0f;
			square[3] = 1.0f; square[4] = 1.0f; square[5] = 0.0f;
			square[6] = 1.0f; square[7] = 0.0f; square[8] = 0.0f;
			square[9] = 0.0f; square[10] = 0.0f; square[11] = 0.0f;
			indices[0] = 0; indices[1] = 1; indices[2] = 2;
			indices[3] = 0; indices[4] = 2; indices[5] = 3;
			g->setVertices(square, 12, 3);
			g->setFaces(GL_TRIANGLES, indices, 6);
			InstancedModel *mo = new InstancedModel(g);
			n->setModel(mo);
			this->addNode(n);
		}
	}
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
}

FBOBuffer* Scene::getFBOBuffer()
{
	return this->FBObuffer;
}

/*void Scene::constructShadowMap()
{

}*/	