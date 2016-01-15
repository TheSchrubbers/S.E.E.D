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
 * \file cubeMap.hpp
 * \author Jérémy RIFFET
 * \version 0.1
 * \copyright Copyright (c) 2015,
 * Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
 * \license Zlib License.
 */

#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

//OTHER INCLUDES
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
//SEED INCLUDES
#include <Seed/Graphics/engine/Constant.hpp>

class Texture;
class Model;
class parserImage;
class Scene;
class Shader;

/*! \class CubeMap
 * \brief Generate a cubemap
 */
class CubeMap
{
public:
	/*!
	 * \brief Constructor of class Texture
	 * \param path : path to the image file
	 * \param scene : scene of the current context
	 * \param flag: if the constructor is a success
	 */
	CubeMap(const std::string path, Scene* scene , unsigned int *flag);
	/*!
	* \brief Destructor of class Texture
	*/
	~CubeMap();
	/*!
	* \brief Draw the cubemap
	*/
	void draw();
	/*!
	* \brief Bind texture of cubemap
	*/
	void bind();
	/*!
	* \brief Release texture of cubemap
	*/
	void release();

private:
	GLuint textureID;
	Shader *shader;
	GLuint VBOVertices;
	GLuint VAO;
	Scene *scene;
	unsigned int block_index_camera;
	/*!
	* \brief load all the textures face of the cubemap
	* \param pathDir: path to the dir which contains the pictures
	* \return true if all the pictures were loading
	*/
	bool loadTextures(const std::string path);
	/*!
	* \brief Creat shader for the cubemap
	*/
	bool createShader();
	/*!
	* \brief create a geometric model for the cube map
	*/
	void createCube();
	/*!
	* \brief Load face's pictures
	* \param type It's the face of the cube (0 = front, 1 = back, 2 = right, 3 = left, 4 = bottom, 5 = top)
	* \param path Path to the picture
	* \param img ParserImage (Optimization)
	*/
	bool loadFace(int type, const std::string *path, parserImage *img);
};

#endif