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

//OTHER INCLUDES
#include <iostream>
#include <vector>
#include <fstream>
#include <assimp/ai_assert.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
//SEED INCLUDES
#include <Seed/Graphics/engine/tools.hpp>

class Geometry;

/*! \class Model
* \brief Loads and generate an Model in GPU
*/
class Model
{
public:
	/*!
	* \brief Constructor of class Model
	* \param mesh: address to the aimesh
	* \param frequency: frequency of updating buffers
	* \param path : path to the model file
	*/
	Model(const aiMesh *mesh, GLuint frequency = GL_STATIC_DRAW, std::string path = "");
	/*!
	* \brief Constructor of class Model
	* \param g: Geometry
	* \param frequency: frequency of updating buffers
	*/
	Model(Geometry *g, GLuint frequency = GL_STATIC_DRAW);
	/*!
	* \brief Destructor of class Model
	*/
	~Model();
	/*!
	* \brief Render the model
	*/
	virtual void render();
	/*!
	* \brief Render the instanced model
	*/
	virtual void render(int nb);
	/*!
	* \brief get the pathname of the mesh
	* \return the path name
	*/
	std::string getPathName();
	void afficher();
	/*!
	* \brief Load datas in GPU
	* \param frequency: frequency updating buffers
	*/
	void loadInGPU(GLuint frequency);

	

protected:

	Geometry *geometry;

	GLuint VBO_vertices, VBO_normals, VBO_tangents, VBO_coordText, VBO_faces;
	GLuint VAO;

	std::string name;
	std::string pathMesh;
};

#endif