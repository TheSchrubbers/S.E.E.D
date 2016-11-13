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
* \file FBOBuffer.hpp
* \brief Frame Buffer Object
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef FBOBUFFER_HPP
#define FBOBUFFER_HPP

//SEED INCLUDES
#include "Seed/Graphics/engine/tools.hpp"
#include "Seed/Graphics/buffers/buffer.hpp"

/*! \class FBOBuffer
* \brief Create, update, delete FBOBuffers
*/
class FBOBuffer : public Buffer
{
public:
	/*!
	* \brief Constructor of class FBOBuffer
	*/
	FBOBuffer();
	/*!
	* \brief Destructor of class FBOBuffer
	*/
	~FBOBuffer();
	/*!
	* \brief delete FBOBuffer
	*/
	void deleteBuffer();
	/*!
	* \brief get Id of the FBO buffer
	* \return ID of the FBO buffer
	*/
	GLuint getID();
	/*!
	* \brief Switch depth texture
	* \param i texture i
	*/
	void switchDepthTexture(int i);
	/*!
	* \brief Create an texture
	* \param format Format of the texture (GL_DEPTH_COMPONENT, GL_COLOR_COMPONENT...)
	* \param type Type of the texture (GL_RGBA, RGB, ...)
	* \param attachment Location which the texture is attached in FBO (GL_DEPTH_ATTACHMENT, GL_COLOR_ATTACHMENT0,...)
	*/
	void createTexture(unsigned int format, unsigned int type, unsigned int attachment);
	/*!
	* \brief Bind FBO to write in the textures attaching
	*/
	void bindWrite();
	/*!
	* \brief Bind FBO for read attaching textures
	* \param texID Id of the attaching texture
	*/
	void bindRead(GLuint texID);
	/*!
	* \brief Release the FBO
	*/
	void release();
	//void activeTextures(GLuint texID);
	/*!
	* \brief Release all the attaching textures
	*/
	void releaseTextures();
	void printTextures();

private:
	std::vector<GLuint> GDepthTextures;
};

#endif