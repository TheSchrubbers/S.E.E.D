#include <Seed/Graphics/material.hpp>

Material::Material(const aiMaterial *material, Scene *sce, const std::string n, unsigned int *flag)
{
	this->scene = sce;
	this->camera = sce->getCamera();
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
Material::Material(Scene *sce, const std::string n, const std::string pathShaders, unsigned int *flag)
{
	this->scene = sce;
	this->camera = sce->getCamera();
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

void Material::pushTexture(Texture *t)
{
	this->textures.push_back(t);
}
void Material::addTexture(const std::string path, Scene *scene, unsigned int type, unsigned int *flag)
{
	std::string p = pathToTextures + path;
	//we verify if the texture is already present
	Texture *t = scene->getCollector()->getTexture(p);
	//if not true
	if (t == NULL)
	{
		//we get a new Texture
		Texture *t = new Texture(p, type, flag);
		//verify if the texture is correct
		if (*flag == SEED_SUCCESS)
		{
			//texture is adding to the collector and the actual material
			scene->getCollector()->collectTextures(t);
			this->pushTexture(t);
		}
		else
		{
			delete t;
		}
	}
	//if t exists
	else
	{
		//we push t in the actual material
		this->pushTexture(t);
	}
}

void Material::setLight(float a, float d, float s)
{
	this->compl.ambiant = a;
	this->compl.diffuse = d;
	this->compl.specular = s;
}

void Material::translate(glm::vec3 T)
{
	this->M = glm::translate(this->M, T);
}