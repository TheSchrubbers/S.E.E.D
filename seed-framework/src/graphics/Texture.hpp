#ifndef TEXTURE_HPP
#define TEXTURE_HPP

/*!
 * \file Texture.hpp
 * \brief Loader Textures from format PNG, BMP, TGA, JPEG, ...
 * \author Jérémy RIFFET
 * \version 0.1
 */

#ifdef _WIN32
	#include <glm\glm.hpp>
	#include <GL\glew.h>
#else
	#include <glm/glm.hpp>
	#include <GL/glew.h>
#endif
#include "parserImage.hpp"
#include <stdlib.h>
#include <stdio.h>

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
	Texture(const char *path);
	/*!
	* \brief Destructor of class Texture
	*/
	~Texture(){};

	/*!
	 * \brief Get width of image
	 * \return Value of width of image
	 */
	int getWidth();
	/*!
	 * \brief Get height of image
	 * \return Value of height of image
	 */
	int getHeight();
	/*!
	 * \brief Get texture ID
	 * \return The GLuint ID of texture
	 */
	GLuint getTextureID();
	/*!
	 * \brief Bind texture from GPU
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
};

#endif