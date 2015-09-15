#include <Seed/Graphics/parserImage.hpp>

parserImage::parserImage()
{}

bool parserImage::readImage(const char* path)
{
	//cimg_library::CImg<unsigned char> test(path);
	std::ifstream imgFile(path, std::ios::binary);
	//type of bmp image
	unsigned short type;

	//open fails?
	if(imgFile)
	{
		char* img;
		int size;

		//get length of file
		imgFile.seekg(0, imgFile.end);
		size = (int)imgFile.tellg();
		imgFile.seekg(0, imgFile.beg);

		//alloc size buf to img array
		img = (char*)malloc(size * sizeof(char));

		//copy image into char* buffer
		imgFile.read(img, size);

		//verify if file has been reading totally
		if(!imgFile)
		{
			printf("ERROR: error reading file %s\n", path);
			imgFile.close();
			return false;
		}

		//close file
		imgFile.close();

		//type of image BMP, 0 if it's not a BMP image
		type = isBMPImage(img);

		//test if image is a bmp image
		if(type > 0)
		{
			//read header of BMP image
			if(!readBMP(this, img, type))
			{
				return false;
			}
		}
	}
	else
	{
		printf("ERROR: Can't read file : %s\n", path);
		return false;
	}
	return true;
}

unsigned short parserImage::isBMPImage(const char* img)
{
	//Bitmap Windows -> BM ; bitmap OS/2 -> BA ; bitmap color icon OS/2 -> CI ; bitmap pointer of color OS/2 -> CP ; bitmap icon OS/2 -> IC ; bitmap pointer OS/2 -> PT
	if(img[0] == 'B')
	{
		if(img[1] == 'M')
		{
			return 1;
		}
		else if(img[1] == 'A')
		{
			return 2;
		}
	}
	else if(img[0] == 'C')
	{
		if(img[1] == 'I')
		{
			return 3;
		}
		else if(img[1] == 'P')
		{
			return 4;
		}
	}
	else if(img[0] == 'I' && img[1] == 'C')
	{
		return 5;
	}
	else if(img[0] == 'P' && img[1] == 'T')
	{
		return 6;
	}
	return 0;
}

bool parserImage::setPixels(char* img, int offset, int size)
{
	this->pixels = (void*)malloc(sizeof(char) * size);
	this->pixels = &img[offset];
	return true;
}

void parserImage::setWidth(unsigned int w)
{
	this->width = w;
}

void parserImage::setHeight(unsigned int h)
{
	this->height = h;
}

void parserImage::setDepth(unsigned short d)
{
	switch (d)
	{
		case 32:
			this->depth = GL_RGBA;
			break;
		case 24:
			this->depth = GL_RGB;
			break;
		default:
			printf("ERROR: format of palet of colors not supported\n");
	}
}

void parserImage::setCompression(unsigned short c)
{
	this->compression = c;
}

unsigned int parserImage::getWidth()
{
	return this->width;
}

unsigned int parserImage::getHeight()
{
	return this->height;
}

unsigned short parserImage::getDepth()
{
	return this->depth;
}

unsigned short parserImage::getCompression()
{
	return this->compression;
}

void* parserImage::getPixels()
{
	return this->pixels;
}
