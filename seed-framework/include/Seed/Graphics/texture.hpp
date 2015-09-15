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
 * \file Texture.hpp
 * \brief Loader Textures from format PNG, BMP, TGA, JPEG, ...
 * \author Jérémy RIFFET
 * \version 0.1
 * \copyright Copyright (c) 2015,
 * Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
 * \license Zlib License.
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "parserImage.hpp"
#include <iostream>

/*! \class Texture
 * \brief Loads and generate an IDTexture in GPU
 */
class Texture
{
public:
	/*!
	 * \brief Constructor of class Texture
	 * \param path : path to the image file
	 */
	Texture(const std::string path, const std::string typeTexture);
	/*!
	* \brief Destructor of class Texture
	*/
	~Texture(){};

	/*!
	 * \brief Get width of image
	 * \return int : Value of width of image
	 */
	int getWidth();
	/*!
	 * \brief Get height of image
	 * \return int : Value of height of image
	 */
	int getHeight();
	/*!
	 * \brief Get texture ID
	 * \return int : The GLuint ID of texture
	 */
	GLuint getTextureID();
	/*!
	 * \brief GLuint : Bind texture from GPU
	 */
	void bind();
	/*!
	 * \brief Release texture from GPU
	 */
	void release();


private:
	int width;
	int height;
	GLuint textureID;
	std::string type;
};

#endif