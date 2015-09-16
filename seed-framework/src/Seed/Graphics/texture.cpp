#include <Seed/Graphics/texture.hpp>

Texture::Texture(const std::string pathT, const unsigned int typeTexture)
{
	parserImage image;
	//init class
	this->width = 0;
	this->height = 0; 

	//load image
	if (image.readImage(pathT.c_str()))
	{
		this->path = pathT;
		this->type = typeTexture;
		//format of image
		//int format = image->getType();
		//format of colours -> RGB RGBA BRG ...
		int t = image.getDepth();
		//width
		int width = image.getWidth();
		//height
		int height = image.getHeight();

		//create a new openGL texture
		glGenTextures(1, &textureID);
		//bind texture to modify this -> typetexture2D
		glBindTexture(GL_TEXTURE_2D, textureID);

		//put texture to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, t, width, height, 0, t, GL_UNSIGNED_BYTE, image.getPixels());
		//texture functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "ERROR: Image \"" << pathT.c_str() << "\" is not found." << std::endl;
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
std::string Texture::getPath()
{
	return this->path;
}
