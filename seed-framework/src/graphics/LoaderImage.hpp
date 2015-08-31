#ifndef LOADER_IMAGE_HPP
#define LOADER_IMAGE_HPP

//#include <corona.h>
#include <iostream>

class Texture;
class LoaderImage
{
public:
	bool loadImage(const char* path, Texture *text);
	//corona::Image *img;
};

#endif