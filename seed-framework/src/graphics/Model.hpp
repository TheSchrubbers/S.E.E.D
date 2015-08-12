#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "Texture.hpp"
#include "Material.hpp"

/*!
* \file Model.hpp
* \brief Load and store Model from format OBJ, ...
* \author Jérémy RIFFET
* \version 0.1
*/

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
	Model(char *path);
	/*!
	* \brief Destructor of class Model
	*/
	~Model(){};

	

private:
	/*!
	* \brief Method who initialize mesh to the scene
	* \param pScene: scene
	* \param path: path to the model file
	*/
	bool InitFromScene(const aiScene* pScene, const char *path);
	bool InitMesh(unsigned int Index, const aiMesh* paiMesh);

	int m_Material;
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec2> m_textCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangentes;
	std::vector<int> m_indicesMaterial;
};

#endif