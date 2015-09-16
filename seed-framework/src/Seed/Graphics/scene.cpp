#include <Seed/Graphics/scene.hpp>

Scene::Scene()
{
	this->rootNode = new Node();
}

Scene::~Scene()
{
	delete rootNode;
	for (int i = 0; i < this->m_models.size(); i++)
	{
		delete this->m_models[i];
	}
}

bool Scene::importModelFromFile(const std::string path, const std::string name)
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
		return false;
	}
	//if file exists
	if (exist)
	{
		//imports model in pScene, the model is triangulate, with tangents informations and merges same vertices
		//A REDEFINIR!!
		const aiScene *pScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);
		
		//if pScene exists, the mesh is assigning with node
		if (pScene)
		{
			if (!loadObjectInScene(pScene, path, name))
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
	return true;
}

bool Scene::loadObjectInScene(const aiScene *pScene, const std::string path, const std::string name)
{
	int i = 0;
	//A MODIFIER PARSER LE FICHIER POUR SAVOIR LE NOM SINON DONNER UN NOM GENERIQUE
	//adding child's node to the root node
	Node *node = new Node(name);
	//set the root node like father node
	node->setFather(this->rootNode);
	//set this node like son's node to the root node
	this->rootNode->addChild(node);

	//load Meshes
	this->loadMeshes(pScene);

	//load Materials
	this->loadMaterials(pScene);

	//insert nodes
	this->insertRecurNode(pScene, pScene->mRootNode, node);
	
	return true;
}

//build the child's tree of nodes of the scene 
void Scene::insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, Node *father)
{
	int i = 0;
	//attribute address's meshe to the node if this is a leaf
	if (nodeFather->mNumMeshes == 1)
	{
		father->setModel(this->m_models[nodeFather->mMeshes[0]]);
		unsigned int indexMaterial = pScene->mMeshes[nodeFather->mMeshes[0]]->mMaterialIndex;
		if (indexMaterial >= 0)
		{
			father->setMaterial(this->m_materials[pScene->mMeshes[nodeFather->mMeshes[0]]->mMaterialIndex]);
		}
	}
	//recursive method for exploring children's nodes and do the same thing
	for (int i = 0; i < nodeFather->mNumChildren; i++)
	{
		Node *n = new Node(father->getName() + "_" + std::to_string(i));
		father->addChild(n);
		n->setFather(father);
		this->insertRecurNode(pScene, nodeFather->mChildren[i], n);
	}
	father->afficher();
}

void Scene::loadMeshes(const aiScene *pScene)
{
	//insert meshes
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		Model *m = new Model(pScene->mMeshes[i]);
		this->m_models.push_back(m);
	}
}

void Scene::loadMaterials(const aiScene *pScene)
{
	aiString pathTexture;
	//insert materials
	for (int i = 0; i < pScene->mNumMaterials; i++)
	{
		//create a new material
		Material *m = new Material(pScene->mMaterials[i], this->getCamera());
		aiString pathTexture;
		std::cout << "Texture : " << pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		if (pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &pathTexture) == AI_SUCCESS)
		{
			std::cout << pathTexture.data << std::endl;
			Texture *t = new Texture(pathTexture.data, TEXTURE_DIFFUSE);
			this->m_textures.push_back(t);
			m->pushTexture(t);
		}
		if (pScene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &pathTexture) == AI_SUCCESS)
		{
			Texture *t = new Texture(pathTexture.data, TEXTURE_SPECULAR);
			this->m_textures.push_back(t);
			m->pushTexture(t);
		}

		this->m_materials.push_back(m);
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

std::vector<Texture*> Scene::getTextures()
{
	return this->m_textures;
}
