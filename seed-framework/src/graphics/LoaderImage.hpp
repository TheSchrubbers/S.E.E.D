#ifndef LOADER_IMAGE_HPP
#define LOADER_IMAGE_HPP
#include "Texture.hpp"
#include <corona.h>

class LoaderImage
{
public:
	bool loadImage(char *path, Texture *text);
	corona::Image *img;
};

#endif