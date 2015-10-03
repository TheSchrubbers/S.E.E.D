#include <Seed/Graphics/texture.hpp>

Texture::Texture(const std::string pathT, const unsigned int typeTexture, unsigned int *flag)
{
	parserImage image;
	/* load an image file directly as a new OpenGL texture */
	/*GLuint tex_2d = SOIL_load_OGL_texture
		(
		pathT.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	if (0 != tex_2d)
	{
		//init class
		this->width = 0;
		this->height = 0; */

	//load image
	if (image.readImage(pathT.c_str()))
	{
		std::cout << "Loading texture " << pathT.c_str() << std::endl;
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

		//this->textureID = tex_2d;
		//create a new openGL texture
		glGenTextures(1, &textureID);
		//bind texture to modify this -> typetexture2D
		glBindTexture(GL_TEXTURE_2D, this->textureID);

		//put texture to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, t, width, height, 0, t, GL_UNSIGNED_BYTE, image.getPixels());
		//texture functions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
		*flag = true;
	}
	else
	{
		*flag = SEED_ERROR_FILE_LOCATION;
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
