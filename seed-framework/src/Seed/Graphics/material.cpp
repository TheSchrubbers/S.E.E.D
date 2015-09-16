#include <Seed/Graphics/material.hpp>

Material::Material(const aiMaterial *material, Camera *cam)
{
	this->camera = cam;
}
Material::Material(Camera *cam)
{
	this->camera = cam;
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