/*!
* \file scene.hpp
* \brief class Scene who contains meshes, materials...
* \author Jérémy RIFFET
* \version 0.1
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "rootnode.hpp"
#include "Node.hpp"
#include <fstream>
#include <string>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <assimp\ai_assert.h>

class Scene
{
public:
	Scene();
	~Scene();
	bool importModelFromFile(char* path);
	bool initMeshInScene(const aiScene *pScene, const char *path);
	void insertRecurNode(const aiNode *nodeFather, Node *father);
	void Scene::bindMeshesToNode(const aiNode *ainode, Node *node);
	//getters
	RootNode* getRootNode();
private:
	RootNode *rootNode;
	std::vector<Model> models;
	std::vector<Material> materials;
	int m_numModels, m_numMaterials;
};
#endif