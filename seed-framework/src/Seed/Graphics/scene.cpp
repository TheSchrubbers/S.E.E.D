#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/node.hpp>
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/node/pointLightNode.hpp>
#include <Seed/Graphics/node/spotLightNode.hpp>
#include <Seed/Graphics/node/directionalLightNode.hpp>
#include <Seed/Graphics/node/flashLightNode.hpp>
#include <Seed/Graphics/light/directionnalLight.hpp>
#include <Seed/Graphics/light/flashLight.hpp>
#include <Seed/Graphics/light/pointLight.hpp>
#include <Seed/Graphics/light/spotLight.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/cubeMap.hpp>
#include <ParticlesSystemMaterial/ParticlesWaterSystemMaterial/ParticlesWaterSystemMaterial.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/Materials/SSAOMaterial/QuadMaterial/QuadMaterial.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>

bool Scene::wireframe = false;
bool Scene::normalMappingActive = true;
bool Scene::specularMapActive = true;
bool Scene::specularMapView = false;
float Scene::deltat = 0.001f;
float Scene::K = 1.0f;
bool Scene::reset = false;
float Scene::radiusNeighbouring = 0.1f;
float Scene::nbParticles = 1;
float Scene::radiusParticle = 0.05f;
float Scene::mu = 3.5f;
float Scene::mass = 0.02f;
float Scene::densityFluid = 998.29f;
float Scene::radiusSphereStarter = 1.0f;
float Scene::threshold = 7.065;
float Scene::sigma = 0.0728;
float Scene::x = 30.0f;
float Scene::AverageNeighbors = 0;
float Scene::sizeCube = 1.0f;
float Scene::mergeCoef = 0.02f;
float Scene::splitCoef = 0.25f;
bool Scene::nextFrame = false;
bool Scene::play = false;
float Scene::nbPart = 0.0f;
bool Scene::half = false;
bool Scene::SPHGravity = true;

Scene::Scene()
{
	this->rootObjectNode = new ObjectNode(this, "RootObjectNode");
	this->rootLightNode = new Node(this, "RootLightNode");
	this->collector = new Collector();
	//create a UBObuffer
	this->camBuf = new UBOBuffer();
	this->camBuf->createBuffer(sizeof(cameraStruct));
	this->cubemap = NULL;

	//this->constructQuad();
	//this->RenderingQuadMaterial = new QuadMaterial(this, "QuadMaterial");

	//this->FBObuffer = new FBOBuffer();
}

Scene::~Scene()
{
	delete rootObjectNode;
	delete collector;
}

ObjectNode* Scene::importModelFromFile(const std::string path, const std::string name)
{
	Assimp::Importer importer;
	bool exist = false;
	//file exists?
	std::ifstream fichier(path);
	if (!fichier.fail())
	{
		exist = true;
		fichier.close();
	}
	else
	{
		std::cout << "ERROR LOADING MODEL : Couldn't open file: " << path << std::endl;
		return nullptr;
	}

	ObjectNode* node = nullptr;

	//if file exists
	if (exist)
	{
		//imports model in pScene, the model is triangulate, with tangents informations and merges same vertices
		//A REDEFINIR!!
		const aiScene *pScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);
		//if pScene exists, the mesh is assigning with node
		if (pScene)
		{
			if ((node = loadObjectInScene(pScene, path, name)) == NULL)
			{
				std::cout << "ERROR LOADING MODEL : error parsing " << path << std::endl << importer.GetErrorString() << std::endl;
				return nullptr;
			}
		}
		else
		{
			std::cout << "ERROR ASSIMP LOADING MODEL : Couln't import Mesh" << std::endl;
			return nullptr;
		}
	}
	return node;
}

ObjectNode* Scene::loadObjectInScene(const aiScene *pScene, const std::string path, const std::string name)
{
	int i = 0;
	//A MODIFIER PARSER LE FICHIER POUR SAVOIR LE NOM SINON DONNER UN NOM GENERIQUE
	//adding child's objectnode to the root objectnode
	ObjectNode *objectNode = new ObjectNode(this, name);
	//set the root node like father node
	//node->setFather(this->rootNode);
	//set this node like son's node to the root node
	//this->rootNode->addChild(node);

	//load Meshes
	this->loadMeshes(pScene, path);

	//load Materials
	//this->loadMaterials(pScene, name);

	//insert nodes
	this->insertRecurNode(pScene, pScene->mRootNode, objectNode);
	
	return objectNode;
}

//build the child's tree of nodes of the scene 
void Scene::insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, ObjectNode *father)
{
	int i = 0;
	//attribute address's meshe to the node if this is a leaf
	if (nodeFather->mNumMeshes == 1)
	{
		father->setModel(this->collector->getModelIndexed(nodeFather->mMeshes[0]));
		/*unsigned int indexMaterial = pScene->mMeshes[nodeFather->mMeshes[0]]->mMaterialIndex;
		if (indexMaterial >= 0)
		{
			father->setMaterial(this->m_materials[pScene->mMeshes[nodeFather->mMeshes[0]]->mMaterialIndex]);
		}*/
	}
	//recursive method for exploring children's nodes and do the same thing
	for (int i = 0; i < nodeFather->mNumChildren; i++)
	{
		ObjectNode *n;
		if (nodeFather->mChildren[i]->mName.C_Str() != "")
		{
			n = new ObjectNode(this, nodeFather->mChildren[i]->mName.C_Str());
		}
		else
		{
			n = new ObjectNode(this, father->getName() + "_" + std::to_string(i));
		}
		father->addChild(n);
		n->setFather(father);
		this->insertRecurNode(pScene, nodeFather->mChildren[i], n);
	}
}

void Scene::loadMeshes(const aiScene *pScene, std::string path)
{
	//insert meshes
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		Model *m = new Model(pScene->mMeshes[i], GL_STATIC_DRAW, path);
		this->collector->collectModels(m);
	}
}

void Scene::setCamera(Camera *cam)
{
	this->camera = cam;
}

//getters
ObjectNode* Scene::getRootObjectNode()
{
	return this->rootObjectNode;
}

Camera* Scene::getCamera()
{
	return this->camera;
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
	return NULL;
}

UBOBuffer* Scene::getCamUBO()
{
	return this->camBuf;
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
	PointLightNode *node = new PointLightNode(this, n);
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
	SpotLightNode *node = new SpotLightNode(this, n);
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
	DirectionnalLightNode *node = new DirectionnalLightNode(this, n);
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
	FlashLightNode *node = new FlashLightNode(this, n);
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

void Scene::cameraUpdate()
{
	//structure of camera
	cameraStruct *cam = new cameraStruct;

	//we set the camera in the structure
	cam->V = this->camera->getViewMatrix();
	cam->P = this->camera->getProjectionMatrix();
	cam->V_inverse = glm::inverse(this->camera->getViewMatrix());

	//send data of lights
	camBuf->updateBuffer(cam, sizeof(cameraStruct));
}

void Scene::collectRenderedNodes()
{
	this->cameraUpdate();
	this->collector->collectRenderedObjectNodes(this->rootObjectNode);
	this->lightsRender();
}

Collector* Scene::getCollector()
{
	return this->collector;
}

void Scene::render()
{
	//update camera
	cameraUpdate();
	//we get all the rendered nodes(models, materials...)
	std::vector<ObjectNode*> *renderedNodes = this->getCollector()->getRenderedCollectedNodes();
	this->lightsRender();
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
	cameraUpdate();
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

void Scene::loadMaterials(const aiScene *pScene, const std::string name)
{
	/*aiString pathTexture;
	unsigned int flag;
	//insert materials
	for (int i = 0; i < pScene->mNumMaterials; i++)
	{
	//create a new material
	Material *m = new Material(pScene->mMaterials[i], this->getCamera(), name);
	aiString pathTexture;
	//std::cout << "Texture : " << pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
	if (pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &pathTexture) == AI_SUCCESS)
	{
	//std::cout << pathTexture.data << std::endl;
	Texture *t = new Texture(pathTexture.data, TEXTURE_DIFFUSE, &flag);
	if (flag == SEED_SUCCESS)
	{
	this->m_textures.push_back(t);
	m->pushTexture(t);
	}
	}
	if (pScene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &pathTexture) == AI_SUCCESS)
	{
	Texture *t = new Texture(pathTexture.data, TEXTURE_SPECULAR, &flag);
	if (flag == SEED_SUCCESS)
	{
	this->m_textures.push_back(t);
	m->pushTexture(t);
	}
	}

	this->m_materials.push_back(m);
	}*/
}

bool Scene::setCubeMap(std::string pathDir)
{
	unsigned int flag;
	this->cubemap = new CubeMap(pathDir, this, &flag);
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
	ObjectNode *n = new ObjectNode(this, name);
	ParticlesWaterSystemMaterial *m = new ParticlesWaterSystemMaterial(this, nb, name + "_Material", flag);
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