#include <Seed/Graphics/material.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/texture.hpp>
#include <Seed/Graphics/cubeMap.hpp>


Material::Material(const aiMaterial *material, Scene *sce, const std::string n, const float reflec, const float refrac, unsigned int *flag)
{
	if (flag == NULL)
	{
		flag = new unsigned int;
	}
	this->scene = sce;
	this->camera = sce->getCamera();
	this->name = n;
	this->mat.Ks = reflec;
	this->mat.Kr = refrac;
	this->texture_normal = NULL;
	this->shader = NULL;
	if (!this->addShaders(pathToDefaultMaterial + "Shaders/"))
	{
		*flag = SEED_ERROR_DEFAULT_SHADER_NOT_FOUND;
	}
	else
	{
		*flag = SEED_SUCCESS;
	}
}
Material::Material(Scene *sce, const std::string n, const float reflec, const float refrac, const std::string pathShaders, unsigned int *flag)
{
	this->scene = sce;
	this->camera = sce->getCamera();
	this->name = n;
	this->mat.Ks = reflec;
	this->mat.Kr = refrac;
	//if user doesn't give path to the shaders, we take the default shaders
	if (pathShaders == "")
	{
		// if default shaders not found, error in the flag else success
		if (!this->addShaders(pathToDefaultMaterial + "Shaders/"))
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
	unsigned int i = 0;
	delete this->shader;
	delete this->camera;
	for (i = 0; i < textures_ambiant.size(); i++)
	{
		delete textures_ambiant[i];
	}
	for (i = 0; i < textures_diffuse.size(); i++)
	{
		delete textures_diffuse[i];
	}
	for (i = 0; i < textures_specular.size(); i++)
	{
		delete textures_specular[i];
	}
}

bool Material::addShaders(const std::string pathDir)
{
	unsigned int *flag = new unsigned int;
	//load shaders
	this->shader = new Shader(pathDir, flag);
	//if shaders not loading we try with default shaders
	if (*flag != SEED_SUCCESS)
	{
		delete shader;
		shader = new Shader(pathToDefaultMaterial + "Shader", flag);
		//if default shaders not found return false
		if (*flag != SEED_SUCCESS)
			return false;
	}
	return true;
}

void Material::pushTexture(Texture *t)
{
	switch(t->getType())
	{
		case SEED_TEXTURE_AMBIANT:
			this->textures_ambiant.push_back(t);
			break;
		case SEED_TEXTURE_DIFFUSE:
			this->textures_diffuse.push_back(t);
			break;
		case SEED_TEXTURE_SPECULAR:
			this->textures_specular.push_back(t);
			break;
		case SEED_TEXTURE_NORMAL:
			this->texture_normal = t;
	}
}
void Material::addTexture(const std::string path, Scene *scene, unsigned int type, unsigned int *flag)
{

	if (flag == NULL)
	{
		flag = new unsigned int;
	}
	std::string p = pathToTextures + path;
	//we verify if the texture is already present
	Texture *t = scene->getCollector()->getTexture(p);

	//if not true
	if (t == NULL)
	{
		//we get a new Texture
		t = new Texture(p, type, flag);
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

void Material::translate(glm::vec3 &T, glm::mat4 &M)

{
	M = glm::translate(M, T);
}

void Material::activeTextures()
{
	GLuint programID = this->shader->getID();
	unsigned int i = 0, j = 0;
	//active and bind textures
	for (i = 0; i < textures_ambiant.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		this->textures_ambiant[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerAmbiantTexture" + std::to_string(i)).c_str()), i);
		j++;
	}
	for (i = 0; i < textures_diffuse.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		this->textures_diffuse[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerDiffuseTexture0" + std::to_string(i)).c_str()), i + j);
		j++;
	}
	for (i = 0; i < this->textures_specular.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		this->textures_specular[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerSpecularTexture" + std::to_string(i)).c_str()), i + j);
		j++;
	}
	if (this->texture_normal)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		this->texture_normal->bind();
		glUniform1i(glGetUniformLocation(programID, "samplerNormalTexture"), j);
		j++;
	}
	CubeMap *c;
	if (c = this->scene->getCubeMap())
	{
		glActiveTexture(GL_TEXTURE0 + j);
		c->bind();
		glUniform1i(glGetUniformLocation(programID, "skybox"), j);
	}
	c = NULL;
}

void Material::releaseTextures()
{
	unsigned int i = 0;
	//release textures
	for (i = 0; i < textures_ambiant.size(); i++)
	{
		this->textures_ambiant[i]->bind();
	}
	for (i = 0; i < textures_diffuse.size(); i++)
	{
		this->textures_diffuse[i]->bind();
	}
	for (i = 0; i < this->textures_specular.size(); i++)
	{
		this->textures_specular[i]->bind();
	}
	CubeMap *c;
	if (c = this->scene->getCubeMap())
	{
		c->release();
	}
}

void Material::printTextures()
{
	std::cout << "Texture ambiant" << std::endl;
	for (int i = 0; i < textures_ambiant.size(); i++)
	{
		std::cout << textures_ambiant[i]->getTextureID() << ", ";
	}
	std::cout << "Texture diffuse" << std::endl;
	for (int i = 0; i < textures_diffuse.size(); i++)
	{
		std::cout << textures_diffuse[i]->getTextureID() << ", ";
	}
	std::cout << "Texture specular" << std::endl;
	for (int i = 0; i < textures_specular.size(); i++)
	{
		std::cout << textures_specular[i]->getTextureID() << ", ";
	}
	std::cout << std::endl;
}

bool Material::activateShader()
{
	if (this->shader->getID() != 0)
	{
		glUseProgram(this->shader->getID());
		return true;
	}
	return false;
}