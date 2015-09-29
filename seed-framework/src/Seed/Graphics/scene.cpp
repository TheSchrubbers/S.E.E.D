#include <Seed/Graphics/scene.hpp>

Scene::Scene()
{
	this->rootNode = new Node(this, "RootNode");
	this->rootNode->addChild(new Node(this, "RootModelNode"));
	this->rootNode->addChild(new Node(this, "RootLightNode"));
	this->collector = new Collector();
	//create a UBObuffer
	this->camBuf = new UBOBuffer();
	this->camBuf->createBuffer(sizeof(cameraStruct));
}

Scene::~Scene()
{
	delete rootNode;
	delete collector;
}

Node* Scene::importModelFromFile(const std::string path, const std::string name)
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
		return NULL;
	}

	Node* node = NULL;

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
				return false;
			}
		}
		else
		{
			std::cout << "ERROR ASSIMP LOADING MODEL : Couln't import Mesh" << std::endl;
			return false;
		}
	}
	return node;
}

Node* Scene::loadObjectInScene(const aiScene *pScene, const std::string path, const std::string name)
{
	int i = 0;
	//A MODIFIER PARSER LE FICHIER POUR SAVOIR LE NOM SINON DONNER UN NOM GENERIQUE
	//adding child's node to the root node
	Node *node = new Node(this, name);
	//set the root node like father node
	//node->setFather(this->rootNode);
	//set this node like son's node to the root node
	//this->rootNode->addChild(node);

	//load Meshes
	this->loadMeshes(pScene, path);

	//load Materials
	//this->loadMaterials(pScene, name);

	//insert nodes
	this->insertRecurNode(pScene, pScene->mRootNode, node);
	
	return node;
}

//build the child's tree of nodes of the scene 
void Scene::insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, Node *father)
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
		Node *n = new Node(this, father->getName() + "_" + std::to_string(i));
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
		Model *m = new Model(pScene->mMeshes[i], path);
		this->collector->collectModels(m);
	}
}

void Scene::setCamera(Camera *cam)
{
	this->camera = cam;
}

//getters
Node* Scene::getRootNode()
{
	return this->rootNode;
}

Camera* Scene::getCamera()
{
	return this->camera;
}

Node* Scene::getNode(const std::string name)
{
	//we use an queue to scan the tree of nodes
	std::queue<Node*> nodes;
	//push the root node
	nodes.push(this->rootNode);
	//we scan all the nodes
	while (!nodes.empty())
	{
		Node *n = nodes.front();
		nodes.pop();
		if (n->getName() == name)
		{
			return n;
		}
		else
		{
			for (int i = 0; i < n->m_children.size(); i++)
			{
				nodes.push(n->m_children[i]);
			}
		}
	}
	return NULL;
}

UBOBuffer* Scene::getLightUBO()
{
	return this->lightBuf;
}

UBOBuffer* Scene::getCamUBO()
{
	return this->camBuf;
}

void Scene::afficher()
{
	std::stack<Node*> nodes;
	nodes.push(this->rootNode);
	while (!nodes.empty())
	{
		Node* n = nodes.top();
		nodes.pop();
		std::cout << n->getName() << " : Model : " << (n->getModel()? 1:0) << ", Material : " << (n->getMaterial()? 1:0) << ", Light : " << (n->getLight()?1:0) << std::endl;
		for (int i = 0; i < n->m_children.size(); i++)
		{
			nodes.push(n->m_children[i]);
		}
	}
}

void Scene::addLight(const glm::vec3 pos, const glm::vec3 c, std::string n)
{
	//search the node root for lights
	Node *light = this->getNode("RootLightNode");
	//if doesn't exists, so we create it
	if (!light)
	{
		light = new Node(this, "RootLightNode");
		this->rootNode->addChild(light);
	}
	//search if the given name is the same of a light in the list of lights
	for (int i = 0; i < light->m_children.size(); i++)
	{
		//if it's true we modify the given name
		if (n == light->m_children[i]->getName())
		{
			n += "_2";
			break;
		}
	}
	//create a new node and push it like the children of node lights
	Node *node = new Node(this, n);
	node->setLight(new Light(c, pos));
	light->addChild(node);
	node->setFather(light);
}

void Scene::lightsRender()
{
	//array of structures of light
	lightStruct *lights;
	//vec3 of colors and positions of light
	glm::vec3 c, p;
	//we get the lights nodes rendering
	std::vector<Node*> *l = this->collector->getLightingRenderedCollectedNodes();
	//number of nodes
	int t = l->size();
	lights = new lightStruct[t];

	//we set the light rendered to the structures of light
	for (int i = 0; i < t; i++)
	{
		c = l->at(i)->getLight()->getColor();
		p = l->at(i)->getLight()->getPosition();
		lights[i].color = glm::vec4(c.x, c.y, c.z, 1.0);
		lights[i].position = glm::vec4(p.x, p.y, p.z, 1.0);
		lights[i].size = glm::ivec4(t);
	}
	//create a UBObuffer
	this->lightBuf = new UBOBuffer();
	this->lightBuf->createBuffer(t * sizeof(lightStruct));
	//send data of lights
	this->lightBuf->updateBuffer(lights, t * sizeof(lightStruct));
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
	this->collector->collectRenderedNodes(this->rootNode);
	this->lightsRender();
}

Collector* Scene::getCollector()
{
	return this->collector;
}

void Scene::render()
{
	cameraUpdate();
	//we get all the rendered nodes(models, materials...)
	std::vector<Node*> *renderedNodes = this->collector->getRenderedCollectedNodes();
	//each node rendering
	for (int i = 0; i < renderedNodes->size(); i++)
	{
		renderedNodes->at(i)->render();
	}
	//this->lightsRender();
}

void Scene::addNode(Node* node)
{
	Node *n = this->getNode("RootModelNode");
	if (!n)
	{
		this->rootNode->addChild(new Node(this, "RootModelNode"));
	}
	n->addChild(node);
	node->setFather(n);
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