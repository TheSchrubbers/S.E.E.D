#include <SSAOMaterial/QuadMaterial/QuadMaterial.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>

QuadMaterial::QuadMaterial(const aiMaterial *material, Scene *sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(material, sce, n, flag, reflec, refrac)
{
	this->init();
}
QuadMaterial::QuadMaterial(Scene *sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac, pathToMaterials + "QuadMaterial/Shaders")
{
	this->init();
}

void QuadMaterial::init()
{
	GLuint programID = this->shader->getID();

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
}

QuadMaterial::~QuadMaterial()
{
}

void QuadMaterial::render(Model *model)
{
	if (this->activateShader())
	{
		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader->getID(), this->block_index_lights[i], i);
		}

		//RENDER;
		//render model
		model->render();

		this->scene->getFBOBuffer()->releaseTextures();
	}
}
