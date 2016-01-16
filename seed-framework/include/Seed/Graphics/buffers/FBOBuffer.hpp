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

//OTHER INCLUDES
#include <GL/glew.h>
//SEED INCLUDES
#include <Seed/Graphics/engine/tools.hpp>

/*! \class FBOBuffer
* \brief Create, update, delete FBOBuffers
*/
class FBOBuffer
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

	void bindWrite();
	void bindRead(GLuint programID);
	void release();
	void activeTextures(GLuint programID);
	void releaseTextures();
	void printTextures();

private:
	GLuint FBOID;
	GLuint GPosition, GNormal, GColorSpec;
	GLuint attachments[3];
	int size;
};

#endif