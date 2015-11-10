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
* \file instancedModel.hpp
* \brief Load and store Model from format OBJ, ...
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef INSTANCEDMODEL_HPP
#define INSTANCEDMODEL_HPP

#include <assimp/ai_assert.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <Seed/Graphics/model/model.hpp>

class Geometry;

/*! \class InstancedModel
* \brief Loads and generate an InstancedModel in GPU
*/
class InstancedModel : public Model
{
public:
	/*!
	* \brief Constructor of class InstancedModel
	* \param mesh: address to the aimesh
	* \param path : path to the model file
	*/
	InstancedModel(const aiMesh *mesh, std::string path);
	/*!
	* \brief Constructor of class InstancedModel
	* \param mesh: address to the aimesh
	* \param path : path to the model file
	*/
	InstancedModel(Geometry *g);
	/*!
	* \brief Destructor of class Model
	*/
	~InstancedModel();
	/*!
	* \brief Render the model
	*/
	void render(int nb);

private:
};

#endif