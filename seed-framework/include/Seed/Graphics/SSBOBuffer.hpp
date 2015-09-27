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
* \file SSBOBuffer.hpp
* \brief geometry of the mesh
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/


#ifndef SSBOBUFFER_HPP
#define SSBOBUFFER_HPP

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

/*! \class SSBOBuffer
* \brief Create, update, delete SSBOBuffers
*/
class SSBOBuffer
{
public:
	/*!
	* \brief Constructor of class SSBOBuffer
	*/
	SSBOBuffer();
	/*!
	* \brief Destructor of class SSBOBuffer
	*/
	~SSBOBuffer();
	/*!
	* \brief Create SSBOBuffer
	* \param size: size of the buffer
	* \return true if the creation is a success
	*/
	void createBuffer(int size);
	/*!
	* \brief Update SSBOBuffer
	* \param data pointer to the datas
	* \param size size of the datas
	*/
	void updateBuffer(void* data, int size);
	/*!
	* \brief delete SBBOBuffer
	*/
	void deleteBuffer();
	/*!
	* \brief get Id of the SSBO buffer
	* \return ID of the SSBo buffer
	*/
	GLuint getID();

private:
	GLuint SSBOID;
	int size;
};

#endif