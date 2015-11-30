#include <ParticlesSystemMaterial/SPHMaterial/SPHMaterial.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/Buffers/UBOBuffer.hpp>
#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>

SPHMaterial::SPHMaterial(Scene *sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac, pathToSPHMaterial + "Shaders")
{
	this->init();
}

void SPHMaterial::init()
{
	if (this->shader)
	{
		GLuint programID = this->shader->getID();
		std::cout << this->shader->getID() << std::endl;
		this->compl.ambiant = 0.1;
		this->compl.diffuse = 0.8;
		this->compl.specular = 0.1;
		// Get a handle for our "MVP" uniform.
		// Only at initialisation time.
		this->matID = glGetUniformLocation(programID, "mat");
		this->compl.ambiantID = glGetUniformLocation(programID, "light.ambiant");
		this->compl.diffuseID = glGetUniformLocation(programID, "light.diffuse");
		this->compl.specularID = glGetUniformLocation(programID, "light.specular");
		this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
		this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
		this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
		this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
		this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
		this->uniformSSBOID = glGetProgramResourceIndex(this->shader->getID(), GL_SHADER_STORAGE_BLOCK, "ParticlesBuffer");
		glShaderStorageBlockBinding(this->shader->getID(), this->uniformSSBOID, 0);
		sph = new SPH(10000, 0.01, 0.1, this->scene);
		this->sph->update();
	}
}

SPHMaterial::~SPHMaterial()
{
}

void SPHMaterial::render(Model *model)
{
	this->sph->algorithm();
	if (this->activateShader())
	{
		//UNIFORMS
		//set the uniform variable MVP
		glUniform1fv(this->compl.ambiantID, 1, &(compl.ambiant));
		glUniform1fv(this->compl.diffuseID, 1, &(compl.diffuse));
		glUniform1fv(this->compl.specularID, 1, &(compl.specular));
		glUniform1i(this->NMACTIVEID, Scene::normalMappingActive);
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
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamUBO()->getID());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 4);
		//bind ssbo particles
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->sph->getSSBOID());

		//RENDER
		//render model
		model->render(this->sph->getNbParticles());
	}
}