#include <Seed/Graphics/material.hpp>

Material::Material(const aiMaterial *material, Camera *cam, const std::string n, unsigned int *flag)
{
	this->camera = cam;
	this->name = n;
	if (!this->addShaders(pathToDefaultShadersDir))
	{
		*flag = SEED_ERROR_DEFAULT_SHADER_NOT_FOUND;
	}
	else
	{
		*flag = SEED_SUCCESS;
	}
}
Material::Material(Camera *cam, const std::string n, const std::string pathShaders, unsigned int *flag)
{
	this->camera = cam;
	this->name = n;
	//if user doesn't give path to the shaders, we take the default shaders
	if (pathShaders == "")
	{
		// if default shaders not found, error in the flag else success
		if (!this->addShaders(pathToDefaultShadersDir))
		{
			*flag = SEED_ERROR_DEFAULT_SHADER_NOT_FOUND;
		}
		else
		{
			*flag = SEED_SUCCESS;
		}
	}
	else
	{
		this->addShaders(pathShaders);
	}
}

Material::~Material()
{
	glDeleteProgram(programID);
	delete this->camera;
	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}
}

bool Material::addShaders(const std::string pathDir)
{
	//load shaders
	this->programID = loadShaders(pathDir + "/VertexShader.hlsl", pathDir + "/FragmentShader.hlsl");
	//if shaders not loading we try with default shaders
	if (!this->programID)
	{
		this->programID = loadShaders(pathToShaders + "default/VertexShader.hlsl", pathToShaders + "default/FragmentShader.hlsl");
		//if default shaders not found return false
		if (!this->programID)
			return false;
	}
	//load shaders in memory
	glUseProgram(programID);
	return true;
}

/*void Material::render(Model *model)
{
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	this->MVP = this->camera->getProjectionMatrix() * camera->getViewMatrix();

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//set the uniform variable MVP
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

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
}*/

void Material::pushTexture(Texture *t)
{
	this->textures.push_back(t);
}
void Material::addTexture(const std::string path, Scene *scene, unsigned int type, unsigned int *flag)
{
	std::string p = pathToTextures + path;
	std::cout << p << std::endl;
	bool find = false;
	for (int i = 0; i < scene->getTextures()->size(); i++)
	{
		if (scene->getTextures()->at(i)->getPath() == p)
		{
			find = true;
			this->pushTexture(scene->getTextures()->at(i));
			break;
		}
	}
	if (!find)
	{
		Texture *t = new Texture(p, type, flag);
		//verify if the texture is correct
		if (*flag == SEED_SUCCESS)
		{
			scene->getTextures()->push_back(t);
			this->pushTexture(t);
		}
		else
		{
			delete t;
		}
	}
}

void Material::setLight(float a, float d, float s)
{
	this->compl.ambiant = a;
	this->compl.diffuse = d;
	this->compl.specular = s;
}