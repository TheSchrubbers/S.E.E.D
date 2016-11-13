//SEED INCLUDES
#include "Seed/Graphics/engine/texture.hpp"
#include "Seed/Graphics/engine/tools.hpp"

using std::endl;
using std::cout;

Texture::Texture(const std::string pathT, const unsigned int typeTexture, unsigned int *flag)
{
	cout << "constructor new texture" << endl;
    initializeOpenGLFunctions();
	
	parserImage image;

	//init class
	this->width = 0;
	this->height = 0;

	cout << "read image" << endl;

	//load image
	if (image.readImage(pathT))
	{
		writeLog("Loading texture " + pathT);
		this->path = pathT;
		this->type = typeTexture;
		//format of image
		//int format = image->getType();
		//format of colours -> RGB RGBA BRG ...
		int t =  image.getDepth();
		//width
		int width = image.getWidth();
		//height
		int height =  image.getHeight();

		cout << "gen texture" << endl;


		//create a new openGL texture
		glGenTextures(1, &textureID);
		//bind texture to modify this -> typetexture2D
		glBindTexture(GL_TEXTURE_2D, this->textureID);

		//put texture to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, t, width, height, 0, t, GL_UNSIGNED_BYTE, image.getPixels());
		//texture functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
		glBindTexture(GL_TEXTURE_2D, 0);
		cout << "fin gen texture" << endl;

		if (flag)
			*flag = SEED_SUCCESS;
	}
	else
	{
		if (flag)
			*flag = SEED_ERROR_FILE_LOCATION;
		writeLog("ERROR -> opening file : " + pathT);
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

unsigned int Texture::getType()
{
	return this->type;
}