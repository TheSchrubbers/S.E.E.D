//SEED INCLUDES
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/node/pointLightNode.hpp>
#include <Seed/Graphics/node/directionalLightNode.hpp>
#include <Seed/Graphics/node/flashLightNode.hpp>
#include <Seed/Graphics/node/spotLightNode.hpp>
#include <Seed/Graphics/lights/pointLight.hpp>
#include <Seed/Graphics/lights/directionnalLight.hpp>
#include <Seed/Graphics/lights/flashLight.hpp>
#include <Seed/Graphics/lights/spotLight.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/model/model.hpp>
#include <Seed/Graphics/engine/texture.hpp>

Collector::Collector()
{
	for (int i = 0; i < 4; i++)
	{
		this->lightBuf[i] = new UBOBuffer();
	}
	this->lightBuf[0]->create(sizeof(pointLightStruct));
	this->lightBuf[1]->create(sizeof(spotLightStruct));
	this->lightBuf[2]->create(sizeof(directionnalLightStruct));
	this->lightBuf[3]->create(sizeof(flashLightStruct));
	pointLightStruct *p = new pointLightStruct;
	spotLightStruct *s = new spotLightStruct;
	directionnalLightStruct *d = new directionnalLightStruct;
	flashLightStruct *f = new flashLightStruct;
	p->attenuation = glm::vec4(0.0);
	p->color = glm::vec4(0.0);
	p->position = glm::vec4(0.0);
	p->size = glm::ivec4(0);
	s->attenuation = glm::vec4(0.0);
	s->color = glm::vec4(0.0);
	s->direction = glm::vec4(0.0);
	s->position = glm::vec4(0.0);
	s->size = glm::vec4(0.0);
	d->color = glm::vec4(0.0);
	d->direction = glm::vec4(0.0);
	d->size = glm::ivec4(0);
	f->attenuation = glm::vec4(0.0);
	f->color = glm::vec4(0.0);
	f->direction = glm::vec4(0.0);
	f->position = glm::vec4(0.0);
	f->size = glm::vec4(0.0);
	this->lightBuf[0]->update(p, sizeof(pointLightStruct));
	this->lightBuf[1]->update(s, sizeof(spotLightStruct));
	this->lightBuf[2]->update(d, sizeof(directionnalLightStruct));
	this->lightBuf[3]->update(f, sizeof(flashLightStruct));
}

Collector::~Collector()
{

}

void Collector::collectRenderedObjectNodes(ObjectNode* node)
{
	std::vector<ObjectNode*> *children = node->getChildren();
	//if the node must be rendered and not is a lightnode
	if (node->isRendered())
	{
		if (node->getName() != "RootLightNode")
		{
			//recursiv method to parse all the nodes
			for (int i = 0; i < children->size(); i++)
			{
				this->collectRenderedObjectNodes(children->at(i));
			}
			//if the node has material and model
			/*if (node->hasMaterial())
			{
				if (node->hasModel())
				this->nodesObjectRenderer.push_back(node);
			}*/
			if (!node->hasChildren())
			{
				this->nodesObjectRenderer.push_back(node);
			}
		}
		/*else
		{
			//recursiv method to parse all the nodes
			for (int i = 0; i < children->size(); i++)
			{
				this->collectRenderedLightingNodes(children->at(i));
			}
		}*/
	}
}

/*void Collector::collectRenderedLightingNodes(Node *node)
{
	//if the node must be rendered and not is a lightnode
	if (node->isRendered())
	{
		//recursiv method to parse all the nodes
		for (int i = 0; i < node->m_children.size(); i++)
		{
			this->collectRenderedLightingNodes(node->m_children[i]);
		}
		//if the node has material and model
		if (node->hasLight())
		{
			this->nodesLightRenderer.push_back(node);
		}
	}
}*/

void Collector::collectMaterials(Material *material)
{
	this->m_materials.push_back(material);
}

void Collector::collectMaterials(std::vector<Material*> materials)
{
	for (int i = 0; i < materials.size(); i++)
	{
		this->m_materials.push_back(materials[i]);
	}
}

void Collector::collectTextures(Texture *texture)
{
	this->m_textures.push_back(texture);
}

void Collector::collectTextures(std::vector<Texture*> textures)
{
	for (int i = 0; i < textures.size(); i++)
	{
		this->m_textures.push_back(textures[i]);
	}
}

void Collector::collectModels(Model* model)
{
	this->m_models.push_back(model);
}

void Collector::collectModels(std::vector<Model*> models)
{
	for (int i = 0; i < models.size(); i++)
	{
		this->m_models.push_back(models[i]);
	}
}

/*void Collector::collectLights(Light* light)
{
	this->m_lights.push_back(light);
}

void Collector::collectLights(std::vector<Light*> lights)
{
	for (int i = 0; i < lights.size(); i++)
	{
		this->m_lights.push_back(lights[i]);
	}
}
*/
Model* Collector::getModelIndexed(int index)
{
	return this->m_models[index];
}
Material* Collector::getMaterialIndexed(int index)
{
	return this->m_materials[index];
}

Texture* Collector::getTexture(const std::string name)
{
	for (int i = 0; i < this->m_textures.size(); i++)
	{
		if (this->m_textures[i]->getPath() == name)
		{
			return this->m_textures[i];
		}
	}
	return NULL;
}

std::vector<ObjectNode*> * Collector::getRenderedCollectedNodes()
{
	return &(this->nodesObjectRenderer);
}

std::vector<Node*> * Collector::getLightingRenderedCollectedNodes()
{
	return &(this->nodesLightRenderer);
}

bool Collector::meshExists(std::string path)
{
	for (int i = 0; i < m_models.size(); i++)
	{
		if (path == m_models[i]->getPathName())
		{
			return true;
		}
	}
	return false;
}

void Collector::pushPointLights()
{
	//array of structures of light
	pointLightStruct *lights;
	int j = 0, i = 0;
	//count nb pointlights rendered
	for (i = 0; i < this->m_pointLightNodes.size(); i++)
	{
		if (this->m_pointLightNodes[i]->isRendered())
		{
			j++;
		}
	}

	if (j > 0)
	{

		//malloc space to the struct pointlight with size j;
		lights = new pointLightStruct[j];

		PointLight* p;
		//push pointlights in the struct
		for (i = 0; i < j; i++)
		{
			p = this->m_pointLightNodes[i]->getLight();
			lights[i].color = glm::vec4(p->getColor(), 1.0);
			lights[i].position = glm::vec4(p->getPosition(), 1.0);
			lights[i].attenuation = glm::vec4(p->getAttenuation(), 0.0);
			lights[i].size = glm::ivec4(j, 0, 0, 0);
			lights[i].K = glm::vec4(p->getK(), 0.0f);
		}

		this->lightBuf[0]->create(j * sizeof(pointLightStruct));
		//send data of lights
		this->lightBuf[0]->update(lights, j * sizeof(pointLightStruct));
	}
}

void Collector::pushSpotLights()
{
	//array of structures of light
	spotLightStruct *lights;
	int j = 0, i = 0;
	//count nb pointlights rendered
	for (i = 0; i < this->m_spotLightNodes.size(); i++)
	{
		if (this->m_spotLightNodes[i]->isRendered())
		{
			j++;
		}
	}

	if (j > 0)
	{

		//malloc space to the struct spotlight with size j;
		lights = new spotLightStruct[j];

		SpotLight* p;
		//push spotlights in the struct
		for (i = 0; i < j; i++)
		{
			p = this->m_spotLightNodes[i]->getLight();
			lights[i].color = glm::vec4(p->getColor(), 1.0);
			lights[i].position = glm::vec4(p->getPosition(), 1.0);
			lights[i].direction = glm::vec4(p->getDirection(), 0.0);
			lights[i].attenuation = glm::vec4(p->getAttenuation(), p->getAngle());
			lights[i].size = glm::ivec4(j, 0, 0, 0);
			lights[i].K = glm::vec4(p->getK(), 0.0f);
		}

		this->lightBuf[1]->create(j * sizeof(spotLightStruct));
		//send data of lights
		this->lightBuf[1]->update(lights, j * sizeof(spotLightStruct));
	}
}

void Collector::pushDirectionnalLights()
{
	//array of structures of light
	directionnalLightStruct *lights;
	int j = 0, i = 0;
	//count nb pointlights rendered
	for (i = 0; i < this->m_directionnalLightNodes.size(); i++)
	{
		if (this->m_directionnalLightNodes[i]->isRendered())
		{
			j++;
		}
	}

	if (j > 0)
	{

		//malloc space to the struct directionnallight with size j;
		lights = new directionnalLightStruct[j];

		DirectionnalLight* p;
		//push directionnallights in the struct
		for (i = 0; i < j; i++)
		{
			p = this->m_directionnalLightNodes[i]->getLight();
			lights[i].direction = glm::vec4(p->getDirection(), 0.0);
			lights[i].color = glm::vec4(p->getColor(), 1.0);
			lights[i].size = glm::ivec4(j, 0, 0, 0);
			lights[i].K = glm::vec4(p->getK(), 0.0f);
		}

		this->lightBuf[2]->create(j * sizeof(directionnalLightStruct));
		//send data of lights
		this->lightBuf[2]->update(lights, j * sizeof(directionnalLightStruct));
	}
}

void Collector::pushFlashLights()
{
	//array of structures of light
	flashLightStruct *lights;
	int j = 0, i = 0;
	//count nb flashlights rendered
	for (i = 0; i < this->m_flashLightNodes.size(); i++)
	{
		if (this->m_flashLightNodes[i]->isRendered())
		{
			j++;
		}
	}

	if (j > 0)
	{

		//malloc space to the struct flashlight with size j;
		lights = new flashLightStruct[j];

		FlashLight* p;
		//push flashlights in the struct
		for (i = 0; i < j; i++)
		{
			p = this->m_flashLightNodes[i]->getLight();
			lights[i].position = glm::vec4(p->getPosition(), 1.0);
			lights[i].direction = glm::vec4(p->getDirection(), 0.0);
			lights[i].color = glm::vec4(p->getColor(), 1.0);
			lights[i].attenuation = glm::vec4(p->getAttenuation(), 0.0);
			lights[i].size = glm::ivec4(j, 0, 0, 0);
			lights[i].K = glm::vec4(p->getK(), 0.0f);
		}

		this->lightBuf[3]->create(j * sizeof(flashLightStruct));
		//send data of lights
		this->lightBuf[3]->update(lights, j * sizeof(flashLightStruct));
	}
}

UBOBuffer* Collector::getLightUBO(int i)
{
	if (i < 4)
		return this->lightBuf[i];
}