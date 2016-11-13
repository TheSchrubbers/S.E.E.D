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
* \file geometry.hpp
* \brief geometry of the mesh
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

//OTHER INCLUDES
#include <iostream>
#include <vector>
#include <fstream>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "Seed/Graphics/definitions.hpp"
/*! \class Geometry
* \brief Structure of geometry of the mesh
*/
class Geometry : openGLFunctions
{
public:
	/*!
	* \brief Constructor of class Geometry
	* \param mesh: mesh's assimp
	*/
	Geometry(const aiMesh *mesh);
	/*!
	* \brief Constructor of class Geometry
	* \param vertices: vector of vertices
	* \param normals: vector of normals
	* \param tangents: vector of tangents
	* \param coord : vector of texture coordinates
	*/
	/*!
	* \brief Constructor of class Geometry
	*/
	Geometry();
	/*!
	* \brief Destructor of class Geometry
	*/
	~Geometry();	
	/*!
	* \brief Has vertices?
	*/
	bool hasVertices();
	/*!
	* \brief Has Normals?
	* \return true if geometry has vertices
	*/
	bool hasNormals();
	/*!
	/ \brief Has Tangents?
	* \return true if geometry has normals
	*/
	bool hasTangents();
	/*!
	* \brief Has TextCoords?
	* \return true if geometry has texture coordinates
	*/
	bool hasTexCoords();
	/*!
	* \brief Has Faces?
	* \return true if geometry has faces
	*/
	bool hasFaces();
	/*!
	* \brief Get Number of vertices
	* \return Number of vertices
	*/
	unsigned int getNumVertices();
	/*!
	* \brief Get Number of faces
	* \return Number of faces
	*/
	unsigned int getNumFaces();
	/*!
	* \brief Get Number of indices by face
	* \return return the number of indeices by face
	*/
	unsigned int getNumIndicesPerFace();
	/*!
	* \brief Get vertices
	* \return Array of vertices
	*/
	std::vector<glm::vec3> * getVertices();
	/*!
	* \brief Get normals
	* \return Array of normals
	*/
	std::vector<glm::vec3> * getNormals();
	/*!
	* \brief Get tangents
	* \return Array of tangents
	*/
	std::vector<glm::vec3> * getTangents();
	/*!
	* \brief Get texture coordinates
	* \return Array of texture coordinates
	*/
	std::vector<glm::vec2> * getTexCoords();
	/*!
	* \brief Get Faces
	* \return Array of faces
	*/
	std::vector<GLuint> * getFaces();
	/*!
	* \brief Set vertices
	* \param vertices vertices' array
	* \param nb number of vertices
	* \param nbVerticesPerFace Number of vertices per face
	*/
	void setVertices(float *vertices, int nb, int nbVerticesPerFace);
	/*!
	* \brief Set normals
	* \param normals : normals' array
	* \param nb : size of array
	*/
	void setNormals(float *normals, int nb);
	/*!
	* \brief Set tangents
	* \param tangents : tangents' array
	* \param nb : size of array
	*/
	void setTangents(float *tangents, int nb);
	/*!
	* \brief Set texture coordinates
	* \param texCoords : texture coords' array
	* \param nb : size of array
	*/
	void setTexCoords(float *texCoords, int nb);
	/*!
	* \brief Set faces
	* \param faces : faces' array
	* \param nb : size of array
	*/
	bool setFaces(GLuint type, GLuint *faces, int nb);

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_textCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<GLuint> m_faces;
	int m_numVertices, m_numFaces, m_maxIndicesByFace;
};

#endif