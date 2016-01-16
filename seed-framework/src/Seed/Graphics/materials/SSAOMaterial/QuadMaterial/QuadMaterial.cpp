//SEED INCLUDES
#include <Seed/Graphics/materials/SSAOMaterial/QuadMaterial/QuadMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>
//OTHER INCLUDES
#include <glm/gtc/random.hpp>


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
	for (int i = 0; i < NBKERNEL; i++)
	{
		this->kernel[i].value = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
		kernel[i].value = glm::normalize(kernel[i].value);
		kernel[i].value *= glm::linearRand(0.0f, 1.0f);
		float scale = (float)i / (float)NBKERNEL;
		scale = 0.1f + scale * scale * 0.9f;
		kernel[i].value *= scale;
	}

	for (int i = 0; i < 4; i++)
	{
		this->noise[i].value = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 0.0f), 0.0f);
		this->noise[i].value = glm::normalize(this->noise[i].value);
	}

	this->uboKernels = new UBOBuffer();
	this->uboKernels->create(NBKERNEL * sizeof(struct Kernel));
	this->uboKernels->update((void*)&this->kernel[0], NBKERNEL * sizeof(struct Kernel));
	/*this->uboNoise = new UBOBuffer();
	this->uboNoise->createBuffer(16 * sizeof(struct Noise));
	this->uboNoise->updateBuffer((void*)&this->noise[0], 16 * sizeof(struct Noise));*/

	GLuint programID = this->shader->getID();

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	this->kernelsID = glGetUniformBlockIndex(programID, "gKernelBuffer");
	//this->noiseID = glGetUniformBlockIndex(programID, "gNoiseBuffer");
}

QuadMaterial::~QuadMaterial()
{
}

void QuadMaterial::render(Model *model)
{
	if (this->activateShader())
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		glClear(GL_COLOR_BUFFER_BIT);
		this->scene->getFBOBuffer()->bindRead(this->shader->getID());

		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader->getID(), this->block_index_lights[i], i);
		}

		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 4);

		glBindBufferBase(GL_UNIFORM_BUFFER, 5, this->uboKernels->getID());
		glUniformBlockBinding(this->shader->getID(), this->kernelsID, 5);

		/*glBindBufferBase(GL_UNIFORM_BUFFER, 6, this->uboNoise->getID());
		glUniformBlockBinding(this->shader->getID(), this->noiseID, 6);*/

		//RENDER;
		//render model
		model->render();
	}
}
