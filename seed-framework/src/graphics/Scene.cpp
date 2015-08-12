#include "Scene.hpp"

Scene::Scene()
{
	rootNode = new Node();
}

Scene::~Scene()
{}

bool Scene::addNode(char* path)
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
		std::printf("ERROR LOADING MODEL : Couldn't open file: \n%s\n", path);
	}
	//if file exists
	if (exist)
	{
		const aiScene *pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
		Node *node = new Node();

		if (pScene)
		{
			if (!InitFromScene(pScene, path))
			{
				printf("ERROR LOADING MODEL : error parsing %s\n %s\n", path, importer.GetErrorString());
			}
		}
	}
}
