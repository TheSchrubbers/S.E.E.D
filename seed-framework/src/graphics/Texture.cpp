#include "Texture.hpp"

Texture::Texture(const char *path)
{
	LoaderImage loader;
	//init class
	this->width = 0;
	this->height = 0; 

	//load image
	if (loader.loadImage(path, this))
	{
		//create a new openGL texture
		glGenTextures(1, &textureID);
		//bind texture to modify this -> typetexture2D
		glBindTexture(GL_TEXTURE_2D, textureID);
		//format of image
		int format = loader.img->getFormat();
		//format of colours -> RGB RGBA BRG ...
		int type = loader.img->getPaletteFormat();
		//width
		int width = loader.img->getWidth();
		//height
		int height = loader.img->getHeight();
		/*float pixs[] = {
			0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
		};*/
		//put texture to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loader.img->getPixels());
		//texture functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//getters
int Texture::getWidth()
{
	return this->width;
}
int Texture::getHeight()
{
	return this->height;
}
GLuint Texture::getTextureID()
{
	return this->textureID;
}

void Texture::bind()
{
	//bind texture to modify this -> typetexture2D
	glBindTexture(GL_TEXTURE_2D, textureID);
}
void Texture::release()
{
	//bind texture to modify this -> typetexture2D
	glBindTexture(GL_TEXTURE_2D, 0);
}
