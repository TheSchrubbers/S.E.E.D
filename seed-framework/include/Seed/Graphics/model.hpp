/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file model.hpp
* \brief Load and store Model from format OBJ, ...
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <assimp/ai_assert.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <Seed/Graphics/material.hpp>
#include <Seed/Graphics/texture.hpp>
#include <Seed/Graphics/Outils.hpp>
#include <Seed/Graphics/shader.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <Seed/Graphics/camera.hpp>
#include <Seed/Graphics/geometry.hpp>

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
	~Model();
	/*!
	* \brief Render the model
	*/
	void render();

	void afficher();

	

private:
	/*std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_textCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<GLuint> m_faces;*/
	Geometry *geometry;
	//int m_numVertices;

	GLuint VBO_vertices, VBO_normals, VBO_tangents, VBO_coordText, VBO_faces;
	GLuint VAO;
};

#endif