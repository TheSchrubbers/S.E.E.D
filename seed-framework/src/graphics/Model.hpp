/*!
* \file Model.hpp
* \brief Load and store Model from format OBJ, ...
* \author Jérémy RIFFET
* \version 0.1
*/


#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>

#ifdef _WIN32
	#include <glm\glm.hpp>
	#include <GL\glew.h>
	#include <assimp\ai_assert.h>
	#include <assimp\postprocess.h>
	#include <assimp\scene.h>
	#include <assimp\Importer.hpp>
#else
	#include <glm/glm.hpp>
	#include <GL/glew.h>
	#include <assimp/ai_assert.h>
	#include <assimp/postprocess.h>
	#include <assimp/scene.h>
	#include <assimp/Importer.hpp>
#endif

#include "Texture.hpp"
#include "Material.hpp"


/*! \class Model
* \brief Loads and generate an Model in GPU
*/
class Model
{
public:
	/*!
	* \brief Constructor of class Model
	* \param path : path to the model file
	*/
	Model(const aiMesh *mesh);
	/*!
	* \brief Destructor of class Model
	*/
	~Model(){};

	

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_textCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<glm::ivec3> m_faces;
	std::vector<int> m_indicesMaterial;
	int m_indexMaterial;
	int m_numVertices;
};

#endif