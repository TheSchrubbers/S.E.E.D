//SEED INCLUDES
#include <Seed/Graphics/materials/ParticlesSystemMaterial/SPHMaterial/SPHMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/engine/shader.hpp>


SPHMaterial::SPHMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac)
{
	//load shaders
	this->shader = std::make_shared<Shader>(pathToMaterials + "SPHMaterial/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		this->init();
	else
		writeLog("Material : " + n + " loading fails");
}

void SPHMaterial::init()
{
	if (this->shader)
	{
		GLuint programID = this->shader->getID();
		this->complight.ambiant = 0.1;
		this->complight.diffuse = 0.8;
		this->complight.specular = 0.1;
		// Get a handle for our "MVP" uniform.
		// Only at initialisation time.
		this->matID = glGetUniformLocation(programID, "mat");
		this->complight.ambiantID = glGetUniformLocation(programID, "light.ambiant");
		this->complight.diffuseID = glGetUniformLocation(programID, "light.diffuse");
		this->complight.specularID = glGetUniformLocation(programID, "light.specular");
		this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
		this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
		this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
		this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
		this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
		this->uniformSSBOID = glGetProgramResourceIndex(this->shader->getID(), GL_SHADER_STORAGE_BLOCK, "ParticlesBuffer");
		glShaderStorageBlockBinding(this->shader->getID(), this->uniformSSBOID, 0);
		sph = new SPH(SPH::nbParticles, SPH::radiusParticle, SPH::radiusNeighbouring, this->scene);
	}
}

SPHMaterial::~SPHMaterial()
{
}

void SPHMaterial::render(Model *model)
{
	if (SPH::reset == true)
	{
		delete this->sph;
		this->sph = new SPH(SPH::nbParticles, SPH::radiusParticle, SPH::radiusNeighbouring, this->scene);
		SPH::reset = false;
	}
	this->sph->algorithm();
	if (this->shader->useProgram())
	{
		//UNIFORMS
		//set the uniform variable MVP
		glUniform1fv(this->complight.ambiantID, 1, &(complight.ambiant));
		glUniform1fv(this->complight.diffuseID, 1, &(complight.diffuse));
		glUniform1fv(this->complight.specularID, 1, &(complight.specular));
		glUniform1i(this->NMACTIVEID, Scene::normalMapActive);
		glUniform1i(this->SMACTIVEID, Scene::specularMapActive);
		glUniform1i(this->SMVIEWID, Scene::specularMapView);
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		//bind ssbo particles
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->sph->getSSBOID());

		//RENDER
		//render model
		model->render(this->sph->getNbParticles());
	}
}