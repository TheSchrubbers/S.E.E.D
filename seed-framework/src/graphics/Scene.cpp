#include "Scene.hpp"

Scene::Scene()
{
	/*this->rootNode = new RootNode();
	m_numModels = 0;
	m_numMaterials = 0;*/
}

Scene::~Scene()
{}

bool Scene::importModelFromFile(char* path)
{
	/*Assimp::Importer importer;
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
		std::printf("ERROR LOADING MODEL : Couldn't open file: \n%s\n", path);
		return false;
	}
	//if file exists
	if (exist)
	{
		//imports model in pScene, the model is triangulate, with tangents informations and merges same vertices
		//A REDEFINIR!!
		const aiScene *pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

		//if pScene exists, the mesh is assigning with node
		if (pScene)
		{
			if (!initMeshInScene(pScene, path))
			{
				printf("ERROR LOADING MODEL : error parsing %s\n %s\n", path, importer.GetErrorString());
				return false;
			}
		}
		else
		{
			printf("ERROR ASSIMP LOADING MODEL : Couln't import Mesh");
			return false;
		}
	}*/
}

bool Scene::initMeshInScene(const aiScene *pScene, const char *path)
{
	/*int i = 0;
	//A MODIFIER PARSER LE FICHIER POUR SAVOIR LE NOM SINON DONNER UN NOM GENERIQUE
	int index = this->rootNode->createNode(pScene->mRootNode->mName.C_Str());
	this->m_numModels += pScene->mNumMeshes;
	this->m_numMaterials += pScene->mNumMaterials;
	//insert meshes
	for (i = 0; i < pScene->mNumMeshes; i++)
	{
		Model m(pScene->mMeshes[i]);
		this->models.push_back(m);
	}
	//insert materials
	for (i = 0; i < pScene->mNumMaterials; i++)
	{
		Material m(pScene->mMaterials[i]);
		this->materials.push_back(m);
	}
	//insert nodes
	this->insertRecurNode(pScene->mRootNode, this->rootNode->getNode(index));
	*/
	return true;
}

//build the child's tree of nodes of the scene 
void Scene::insertRecurNode(const aiNode *nodeFather, Node *father)
{
	/*int i = 0;
	//attribute index's meshes to the node
	for (i = 0; i < nodeFather->mNumMeshes; i++)
	{
		father->addModel(nodeFather->mMeshes[i]);
	}
	//recursive method for exploring children's nodes and do the same thing
	for (int i = 0; i < nodeFather->mNumChildren; i++)
	{
		Node *n = new Node();
		father->addChild(n);
		this->insertRecurNode(nodeFather->mChildren[i], n);
	}*/
}

void Scene::bindMeshesToNode(const aiNode *ainode, Node *node)
{
	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		
	}
}

//getters
RootNode* Scene::getRootNode()
{
	return this->rootNode;
}
