//SEED INCLUDES
#include <Seed/Graphics/materials/ParticlesSystemMaterial/ParticlesWaterSystemMaterial/ParticlesWaterSystemMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/particles/particlesSystem.hpp>
#include <Seed/Graphics/particles/particlesWaterSystem.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ParticlesWaterSystemMaterial::ParticlesWaterSystemMaterial(std::shared_ptr<Scene> sce, const int &nb, const std::string n, unsigned int *flag) : Material(sce, n, flag, 0.0, 0.0, pathToParticlesWaterSystemMaterial + "Shaders")
{
	this->nbParticles = nb;
	this->init();
	ParticlesWaterSystem *system = new ParticlesWaterSystem(sce, this->nbParticles, SEED_POINT, glm::vec3(0.0));
}

void ParticlesWaterSystemMaterial::init()
{
	this->M = glm::mat4(1.0);
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(this->shader->getID(), "M");
	this->block_index_camera = glGetUniformBlockIndex(this->shader->getID(), "CameraBuffer");
}

ParticlesWaterSystemMaterial::~ParticlesWaterSystemMaterial()
{
}

void ParticlesWaterSystemMaterial::render(Model *model)
{
	InstancedModel *m = dynamic_cast<InstancedModel*>(model);
	if (this->activateShader())
	{
		//UNIFORMS
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		//OPTIONS
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//TEXTURES
		//this->activeTextures();

		//BUFFERS
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 0);

		//RENDER
		//render model
		m->render(this->nbParticles);

		//RELEASE
		//this->releaseTextures();
	}
}