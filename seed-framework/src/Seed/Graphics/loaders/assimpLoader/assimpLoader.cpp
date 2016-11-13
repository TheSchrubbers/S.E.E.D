//SEED INCLUDES
#include <Seed/Graphics/loaders/assimpLoader/assimpLoader.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/model/model.hpp>
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/engine/tools.hpp>

//OTHER INCLUDES

AssimpLoader::AssimpLoader()
{

}

AssimpLoader::~AssimpLoader()
{

}

ObjectNode* AssimpLoader::importModelFromFile(const std::string path, std::shared_ptr<Scene> scene, Collector *collector, const std::string name)
{
	this->offset = collector->getNbModels();
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
		writeLog("ERROR LOADING MODEL : Couldn't open file: " + path);
		return nullptr;
	}

	ObjectNode* node = nullptr;

	//if file exists
	if (exist)
	{
		//imports model in pScene, the model is triangulate, with tangents informations and merges same vertices
		//A REDEFINIR!!
		const aiScene *pScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);//aiProcessPreset_TargetRealtime_Fast);
		//if pScene exists, the mesh is assigning with node
		if (pScene)
		{
			if ((node = loadObjectInScene(pScene, path, scene, collector, name)) == nullptr)
			{
				writeLog("ERROR LOADING MODEL : error parsing " + path + "\n" + importer.GetErrorString());
				return nullptr;
			}
			else
			{
				writeLog("LOADING MODEL SUCCESS : " + path);
			}
		}
		else
		{
			writeLog("ERROR ASSIMP LOADING MODEL : Couln't import Mesh");
			return nullptr;
		}
	}
	return node;
}

ObjectNode* AssimpLoader::loadObjectInScene(const aiScene *pScene, const std::string path, std::shared_ptr<Scene> scene, Collector *collector, const std::string name)
{
	int i = 0;
	//A MODIFIER PARSER LE FICHIER POUR SAVOIR LE NOM SINON DONNER UN NOM GENERIQUE
	//adding child's objectnode to the root objectnode
	ObjectNode *objectNode = new ObjectNode(scene, name);
	objectNode->setShadowMapped(true);
	//set the root node like father node
	//node->setFather(this->rootNode);
	//set this node like son's node to the root node
	//this->rootNode->addChild(node);

	//load Meshes
	this->loadMeshes(pScene, collector, path);

	//load Materials
	//this->loadMaterials(pScene, name);

	//insert nodes
	this->insertRecurNode(pScene, pScene->mRootNode, objectNode, scene, collector);
	
	return objectNode;
}

void AssimpLoader::insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, ObjectNode *father, std::shared_ptr<Scene> scene, Collector *collector)
{
	int i = 0;
	//attribute address's meshe to the node if this is a leaf
	if (nodeFather->mNumMeshes == 1)
	{
		father->setModel(collector->getModelIndexed(nodeFather->mMeshes[0] + this->offset));
	}
	//recursive method for exploring children's nodes and do the same thing
	for (int i = 0; i < nodeFather->mNumChildren; i++)
	{
		ObjectNode *n;
		if (nodeFather->mChildren[i]->mName.C_Str() != "")
		{
			n = new ObjectNode(scene, nodeFather->mChildren[i]->mName.C_Str());
		}
		else
		{
			n = new ObjectNode(scene, father->getName() + "_" + std::to_string(i));
		}
		father->addChild(n);
		n->setFather(father);
		n->setShadowMapped(true);
		this->insertRecurNode(pScene, nodeFather->mChildren[i], n, scene, collector);
	}
}

void AssimpLoader::loadMeshes(const aiScene *pScene, Collector *collector, std::string path)
{
	//insert meshes
	for (int i = 0; i < pScene->mNumMeshes; i++)
	{
		Model *m = new Model(pScene->mMeshes[i], GL_STATIC_DRAW, path);
		collector->collectModels(m);
	}
}