/*
* Copyright (c) <2015> <Antoine Richard and J�r�my Riffet, S.E.E.D>
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
* \author J�r�my RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and J�r�my "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include <assimp/scene.h>
#include <GL\glew.h>
#include <glm/glm.hpp>

/*! \class Geometry
* \brief Structure of geometry of the mesh
*/
class Geometry
{
public:
	/*!
	* \brief Constructor of class Geometry
	* \param mesh: mesh's assimp
	*/
	Geometry(const aiMesh *mesh);
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
	unsigned int getNumIndices();
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

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_textCoords;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<GLuint> m_faces;
	int m_numVertices, m_numFaces, m_maxIndicesByFace;
};

#endif