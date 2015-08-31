#include "LoaderImage.hpp"

bool LoaderImage::loadImage(const char *path, Texture *text)
{
	return true;
}
/*
	img = corona::OpenImage(path, corona::PF_R8G8B8A8);
	if (!img)
	{
		printf("ERROR : LOADER_IMAGE : Can't load Image : %s in memory.\n", path);
		return false;
	}
	else
	{
		/*unsigned char* p = (unsigned char*)img->getPixels();
		for (int i = 0; i < 1000; i++)
		{
			printf("%f\n", (float)p[i * 4 + 3]);
		}
		text->setHeight(img->getHeight());
		text->setWidth(img->getWidth());

		unsigned char *p = (unsigned char*)img->getPixels();
		text->setDataSize(text->getWidth(), text->getHeight());
		text->setData(p);*/
		/*return true;
	}
}*/