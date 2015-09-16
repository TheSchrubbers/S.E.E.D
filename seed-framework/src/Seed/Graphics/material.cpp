#include <Seed/Graphics/material.hpp>

Material::Material(const aiMaterial *material, Camera *cam, const std::string n)
{
	this->camera = cam;
	this->name = n;
}
Material::Material(Camera *cam, const std::string n)
{
	this->camera = cam;
	this->name = n;
}

void Material::addShaders(const std::string pathDir)
{
	this->programID = loadShaders(pathToShaders + pathDir + "/VertexShader.hlsl", pathToShaders + pathDir + "/FragmentShader.hlsl");
}

void Material::render(Model *model)
{
	int i = 0;
	int nbTextures = this->textures.size();
	//active and bind textures
	for (i = 0; i < nbTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		this->textures[i]->bind();
	}
	//render model
	model->render();

	//release textures
	for (i = 0; i < nbTextures; i++)
	{
		this->textures[i]->release();
	}
}

void Material::pushTexture(Texture *t)
{
	this->textures.push_back(t);
}
void Material::addTexture(const std::string path, Scene *scene, unsigned int type)
{
	bool find;
	for (int i = 0; i < scene->getTextures().size(); i++)
	{
		if (scene->getTextures()[i]->getPath() == path)
		{
			find = true;
			this->pushTexture(scene->getTextures()[i]);
			break;
		}
	}
	if (!find)
	{
		Texture *t = new Texture(path, type);
		scene->getTextures().push_back(t);
		this->pushTexture(t);
	}
}