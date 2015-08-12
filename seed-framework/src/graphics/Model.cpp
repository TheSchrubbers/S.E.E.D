#include "Model.hpp"
#include <fstream>
#include "..\..\..\lib\glm\glm\glm.hpp"
#include "..\..\..\lib\glew-1.12.0\include\GL\glew.h"

Model::Model(char *path)
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
		pScene->mRootNode->
		if (pScene)
		{
			if (!InitFromScene(pScene, path))
			{
				printf("ERROR LOADING MODEL : error parsing %s\n %s\n", path, importer.GetErrorString());
			}
		}
	}
}

//Initialisation from aiscene
bool Model::InitFromScene(const aiScene* pScene, const char *path)
{
	pScene->mRootNode->mMeshes
	//resize vectors with the number of meshes and textures
	m_Entries.resize(pScene->mNumMeshes);
	//m_Textures.resize(pScene->mNumTextures);

	//Initialize each model
	for (int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
	}
	return true;
	//return initMaterials(pScene, path);
}

bool Model::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	//affect a material to the index entry of vector of materials
	m_Material = paiMesh->mMaterialIndex;

	//get each pos, normal, uv for the current i vertex
	for (int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pos = &(paiMesh->mVertices[i]);
		const aiVector3D* normal = &(paiMesh->mNormals[i]);
		const aiVector3D* uv = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &(aiVector3D(0.0f, 0.0f, 0.0f));
		Vertex v(glm::vec3(pos->x, pos->y, pos->z), glm::vec3(normal->x, normal->y, normal->z), glm::vec2(uv->x, uv->y));
		m_Vertices.push_back(v);
	}
	for (int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		m_Indices.push_back(Face.mIndices[0]);
		m_Indices.push_back(Face.mIndices[1]);
		m_Indices.push_back(Face.mIndices[2]);
	}
}
