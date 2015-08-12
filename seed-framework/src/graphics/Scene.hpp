#ifndef SCENE_HPP
#define SCENE_HPP

#include "Node.hpp"
#include "..\..\..\lib\assimp\include\assimp\Importer.hpp"
#include "..\..\..\lib\assimp\include\assimp\ai_assert.h"
#include "..\..\..\lib\assimp\include\assimp\postprocess.h"
#include "..\..\..\lib\assimp\include\assimp\scene.h"
#include <fstream>

class Scene
{
public:
	Scene();
	~Scene();
	bool addNode(char* path);
private:
	Node *rootNode;
	std::vector<Model> models;
	std::vector<Material> materials;
};
#endif