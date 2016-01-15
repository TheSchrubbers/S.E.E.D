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
 * \file parserImage.hpp
 * \brief Parser images BMP, PNG, JPG, TAR
 * \author Jérémy RIFFET
 * \version 0.1
 * \copyright Copyright (c) 2015,
 * Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
 * \license Zlib License.
 */

#ifndef PARSERIMAGE_HPP
#define PARSERIMAGE_HPP

//OTHER INCLUDES
#include <GL/glew.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
 //SEED INCLUDES
#include <Seed/Graphics/loaders/BMPParser.hpp>
#include <Seed/Graphics/engine/Constant.hpp>
#include <Seed/Graphics/loaders/imageLoading/png.hpp>
#include <Seed/Graphics/loaders/imageLoading/tga.hpp>
 
//! CLASSNAME parserImage
 /*!
  * \brief
  * parsing multiplies types of images.
  */
class parserImage
{
	private:
		//attributes
		void* pixels;
		unsigned int width, height;
		unsigned short depth, compression;
		//methods
		/*!
		 * \brief
		 * Method verifies if the image is a BMP image
		 * \param img : image's pointer
		 * \return a short who's the type of BMP
		 * Bitmap Windows : 1 ; bitmap OS/2 : 2 ; bitmap color icon OS/2 : 3 ; bitmap pointer of color OS/2 : 4 ; bitmap icon OS/2 : 5; bitmap pointer OS/2 : 6
		 */
		unsigned short isBMPImage(const char* img);
	public:
		//! Default constructor
		parserImage();
		/*!
		 * \brief 
		 * Method reads an image
		 * \param path : the path of the image
		 * \return an boolean if the image load correctly
		 */
		bool readImage(const std::string path);

		//setters
		void setWidth(unsigned int);
		void setHeight(unsigned int);
		void setDepth(unsigned short);
		void setCompression(unsigned short);
		bool setPixels(char*, int, int);
		//getters
		unsigned int getWidth();
		unsigned int getHeight();
		unsigned short getDepth();
		unsigned short getCompression();
		void* getPixels();

		
		int getFormat(const std::string path);

		//parsers
		bool BMP_parser(const std::string path);
		bool PNG_parser(const std::string path);
		bool TGA_parser(const std::string path);
};

#endif