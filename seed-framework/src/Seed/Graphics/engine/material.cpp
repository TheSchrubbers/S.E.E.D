//SEED INCLUDES
#include "Seed/Graphics/engine/material.hpp"
#include "Seed/Graphics/engine/scene.hpp"
#include "Seed/Graphics/engine/collector.hpp"
#include "Seed/Graphics/engine/texture.hpp"
#include "Seed/Graphics/engine/cubeMap.hpp"
#include "Seed/Graphics/engine/shader.hpp"

using std::cout;
using std::endl;

Material::Material(const aiMaterial *material, std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac)
{
    initializeOpenGLFunctions();

	m_scene = sce;
	m_camera = sce->getCamera();
	m_name = n;
	m_mat.Ks = reflec;
	m_mat.Kr = refrac;
	m_texture_normal = nullptr;
	m_texture_depthMap = nullptr;
}
Material::Material(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac)
{
    initializeOpenGLFunctions();
	
	m_scene = sce;
	m_camera = sce->getCamera();
	m_name = n;
	m_mat.Ks = reflec;
	m_mat.Kr = refrac;
	m_texture_normal = nullptr;
	m_texture_depthMap = nullptr;
}

Material::~Material()
{
	m_camera = nullptr;
	unsigned int i = 0;
	for (i = 0; i < m_textures_ambiant.size(); i++)
		delete m_textures_ambiant[i];
	for (i = 0; i < m_textures_diffuse.size(); i++)
		delete m_textures_diffuse[i];
	for (i = 0; i < m_textures_specular.size(); i++)
		delete m_textures_specular[i];
	delete m_texture_normal;
	delete m_texture_depthMap;
}

void Material::pushTexture(Texture *t)
{
	switch(t->getType())
	{
		case SEED_TEXTURE_AMBIANT:
			m_textures_ambiant.push_back(t);
			break;
		case SEED_TEXTURE_DIFFUSE:
			m_textures_diffuse.push_back(t);
			break;
		case SEED_TEXTURE_SPECULAR:
			m_textures_specular.push_back(t);
			break;
		case SEED_TEXTURE_NORMAL:
			m_texture_normal = t;
			break;
		case SEED_TEXTURE_DEPTHMAP:
			m_texture_depthMap = t;
			break;
	}
}
void Material::addTexture(const std::string path, std::shared_ptr<Scene> scene, unsigned int type, unsigned int *flag)
{
	cout << "add texture" << endl;
	unsigned int flag2;
	std::string p = pathToTextures + path;
	//we verify if the texture is already present
	cout << "constructor texture" << endl;
	Texture *t = m_scene->getCollector()->getTexture(p);

	//if not true
	if (t == nullptr)
	{
		cout << "constructor texture if t == nullptr" << endl;
		//we get a new Texture
		t = new Texture(p, type, &flag2);
		//verify if the texture is correct
		if (flag2 == SEED_SUCCESS)
		{
			cout << "colelct textures" << endl;

			//texture is adding to the collector and the actual material
			scene->getCollector()->collectTextures(t);
			cout << "push textures" << endl;

			pushTexture(t);
		}
		else
			delete t;
	}
	//if t exists
	else
	{
		cout << "push texture else " << endl;
		//we push t in the actual material
		pushTexture(t);
	}
	cout << "flag" << endl;
	if(flag)
		*flag = flag2;
}

void Material::setLight(float a, float d, float s)
{
	m_complight.ambiant = a;
	m_complight.diffuse = d;
	m_complight.specular = s;
}

void Material::activeTextures(GLuint programID)
{
	unsigned int i = 0, j = 0;
	//active and bind textures
	for (i = 0; i < m_textures_ambiant.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		m_textures_ambiant[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerAmbiantTexture" + std::to_string(i)).c_str()), i);
		j++;
	}
	for (i = 0; i < m_textures_diffuse.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		m_textures_diffuse[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerDiffuseTexture" + std::to_string(i)).c_str()), i + j);
		j++;
	}
	for (i = 0; i < m_textures_specular.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		m_textures_specular[i]->bind();
		glUniform1i(glGetUniformLocation(programID, ("samplerSpecularTexture" + std::to_string(i)).c_str()), i + j);
		j++;
	}
	if (m_texture_normal)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		m_texture_normal->bind();
		glUniform1i(glGetUniformLocation(programID, "samplerNormalTexture"), j);
		j++;
	}
	if (m_texture_depthMap)
	{
		glActiveTexture(GL_TEXTURE0 + j);
		m_texture_depthMap->bind();
		glUniform1i(glGetUniformLocation(programID, "samplerDepthMapTexture"), j);
		j++;
	}
	CubeMap *c;
	if (c = m_scene->getCubeMap())
	{
		glActiveTexture(GL_TEXTURE0 + j);
		c->bind();
		glUniform1i(glGetUniformLocation(programID, "skybox"), j);
	}
	c = nullptr;
}

void Material::releaseTextures()
{
	unsigned int i = 0;
	//release textures
	for (i = 0; i < m_textures_ambiant.size(); i++)
	{
		m_textures_ambiant[i]->release();
	}
	for (i = 0; i < m_textures_diffuse.size(); i++)
	{
		m_textures_diffuse[i]->release();
	}
	for (i = 0; i < m_textures_specular.size(); i++)
	{
		m_textures_specular[i]->release();
	}
	if (m_texture_normal)
	{
		m_texture_normal->release();
	}
	CubeMap *c;
	if (c = m_scene->getCubeMap())
	{
		c->release();
	}
	c = nullptr;
}

void Material::printTextures()
{
	std::cout << "Texture ambiant" << std::endl;
	for (int i = 0; i < m_textures_ambiant.size(); i++)
	{
		std::cout << m_textures_ambiant[i]->getTextureID() << ", ";
	}
	std::cout << "Texture diffuse" << std::endl;
	for (int i = 0; i < m_textures_diffuse.size(); i++)
	{
		std::cout << m_textures_diffuse[i]->getTextureID() << ", ";
	}
	std::cout << "Texture specular" << std::endl;
	for (int i = 0; i < m_textures_specular.size(); i++)
	{
		std::cout << m_textures_specular[i]->getTextureID() << ", ";
	}
	std::cout << std::endl;
}

glm::mat4 Material::getModelMatrix()
{
	return m_M;
}