/**
 * File: parserImage.hpp
 * Author jeremy riffet
 * 
 */

#ifndef PARSERIMAGE_HPP
#define PARSERIMAGE_HPP

#include <GL/glew.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <Seed/Graphics/BMPParser.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <Seed/Graphics/imageLoading/png.hpp>
#include <Seed/Graphics/imageLoading/tga.hpp>
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