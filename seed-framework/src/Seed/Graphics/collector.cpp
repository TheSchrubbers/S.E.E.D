#include <Seed/Graphics/collector.hpp>

Collector::Collector()
{

}

Collector::~Collector()
{

}

void Collector::collectRenderedNodes(Node* node)
{
	//if the node must be rendered and not is a lightnode
	if (node->isRendered())
	{
		if (node->getName() != "RootLightNode")
		{
			//recursiv method to parse all the nodes
			for (int i = 0; i < node->m_children.size(); i++)
			{
				this->collectRenderedNodes(node->m_children[i]);
			}
			//if the node has material and model
			if (node->hasMaterial())
			{
				if (node->hasModel())
					this->nodesRenderer.push_back(node);
			}
		}
		else
		{
			//recursiv method to parse all the nodes
			for (int i = 0; i < node->m_children.size(); i++)
			{
				this->collectRenderedLightingNodes(node->m_children[i]);
			}
		}
	}
}

void Collector::collectRenderedLightingNodes(Node *node)
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
}

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

void Collector::collectLights(Light* light)
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

std::vector<Node*> * Collector::getRenderedCollectedNodes()
{
	return &(this->nodesRenderer);
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