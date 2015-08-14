/*!
* \file model.cpp
* \author Jérémy RIFFET
* \version 0.1
*/

#include "Model.hpp"
#include <fstream>
#include "..\..\..\lib\glm\glm\glm.hpp"
#include "..\..\..\lib\glew-1.12.0\include\GL\glew.h"
#include "..\..\..\lib\assimp\include\assimp\ai_assert.h"
#include "..\..\..\lib\assimp\include\assimp\postprocess.h"
#include "..\..\..\lib\assimp\include\assimp\scene.h"

Model::Model(const aiMesh *mesh)
{
	int i = 0;
	this->m_numVertices = mesh->mNumVertices;
	for (i = 0; i < this->m_numVertices; i++)
	{
		//copy vertices
		this->m_vertices[i].x = mesh->mVertices[i].x;
		this->m_vertices[i].y = mesh->mVertices[i].y;
		this->m_vertices[i].z = mesh->mVertices[i].z;
		//copy tangents
		this->m_tangents[i].x = mesh->mTangents[i].x;
		this->m_tangents[i].y = mesh->mTangents[i].y;
		this->m_tangents[i].z = mesh->mTangents[i].z;
		//copy texture coordinates
		this->m_textCoords[i].x = mesh->mTextureCoords[i]->x;
		this->m_textCoords[i].y = mesh->mTextureCoords[i]->y;
		//copy normals
		this->m_normals[i].x = mesh->mNormals[i].x;
		this->m_normals[i].y = mesh->mNormals[i].y;
		this->m_normals[i].z = mesh->mNormals[i].z;
	}

	for (i = 0; i < mesh->mNumFaces; i++)
	{
		//copy faces
		//this->m_faces[i].x = mesh->mFaces[i].mIndices
	}

	this->m_indexMaterial = mesh->mMaterialIndex;

}


