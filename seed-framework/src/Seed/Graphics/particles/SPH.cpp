#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/model/instancedModel.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/data_structure/KDtree.hpp>

SPH::SPH()
{
	//assimp load sphere
	Assimp::Importer importer;
	std::string path = pathToModels + "UVsphereLow.obj";
	const aiScene *pScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);
	this->nbParticles = 1000;
	//set the starter of the particles
	this->starter = new Starter();
	//load sphare mesh
	sphere = new InstancedModel(pScene->mMeshes[0], path);
	unsigned int flag;
	const std::string s = pathToMaterials + "SimpleMaterial/Shaders/";
	//new shader -> simplematerial
	this->shader = new Shader(s, &flag);
	scanSeedError(flag);
	//get the shader's camera
	this->block_index_camera = glGetUniformBlockIndex(shader->getID(), "CameraBuffer");
	//get the lights' shader
	this->block_index_lights[0] = glGetUniformBlockIndex(this->shader->getID(), "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(this->shader->getID(), "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(this->shader->getID(), "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(this->shader->getID(), "FlashLightsBuffer");
	this->NMID = glGetUniformLocation(this->shader->getID(), "Normal_Matrix");
	//get the shader's particles
	this->SSBOID = glGetProgramResourceIndex(this->shader->getID(), GL_SHADER_STORAGE_BLOCK, "ParticlesBuffer");
	glShaderStorageBlockBinding(this->shader->getID(), this->SSBOID, 0);
	//create system
	this->createSystem(0.01);
}

SPH::~SPH()
{
	delete this->starter;
}

void SPH::print()
{

}

void SPH::loadSystem()
{
	this->SSBOParticles->updateBuffer((void*)(&this->particles[0]), this->nbParticles * sizeof(ParticleSPH));
}

void SPH::createSystem(float r)
{
	//starter of particles in sphere
	std::vector<glm::vec3> pos = this->starter->addSphereStarter(glm::vec3(0.0), 0.5, this->nbParticles);
	//for each particle we set the parameters
	for (int i = 0; i < this->nbParticles; i++)
	{
		ParticleSPH *p = new ParticleSPH;
		//position of the particle i
		p->position = glm::vec4(pos[i], r);
		//transformation matrix of the particle i
		p->M = glm::mat4(1.0);
		//translation of the particle i
		translate(p->M, glm::vec3(pos[i].x, pos[i].y, pos[i].z));
		//scale of the particle i
		scale(p->M, glm::vec3(r));
		//inverse transformation matrix of the particle i
		p->inverseM = glm::transpose(glm::inverse(p->M));
		//color of the particle i
		p->color = glm::vec4(1.0);
		//we push the particle i into the array of particles
		particles.push_back(p);
	}
	//SSBO creating
	//new SSBObuffer
	this->SSBOParticles = new SSBOBuffer();
	//create a SSBOBuffer with a size (number of particles * size of the strcut of a particle)
	this->SSBOParticles->createBuffer(this->nbParticles * sizeof(ParticleSPH));
	std::vector<ParticleSPH> v;
	for (ParticleSPH *pp : this->particles)
	{
		v.push_back(*pp);
	}
	//setting particles into the ssbo buffer
	this->SSBOParticles->updateBuffer(&v[0], this->nbParticles * sizeof(ParticleSPH));
	//system("pause");
	//create KDTree for neighbouring particles
	this->kdtree = new KDtree(particles, 10);

	float rad = 0.5;
	std::vector<ParticleSPH*> p = this->kdtree->radiusNeighbouring(this->particles[0], rad);
	this->particles[0]->color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	for (int i = 0; i < p.size(); i++)
	{
		//std::cout << p[i]->position.x << " " << p[i]->position.y << " " << p[i]->position.z << std::endl;
		p[i]->color = glm::vec4(1.0, 0.0, 0.0, 1.0);
	}
	this->updateSystem();
}

void SPH::updateSystem()
{
	this->SSBOParticles->bind();
	//get the SSBO data
	ParticleSPH *p = (ParticleSPH*)this->SSBOParticles->getData(sizeof(ParticleSPH) * this->nbParticles);
	if (p)
	{
		//update the SSBO
		for (int i = 0; i < this->nbParticles; i++)
		{
			p[i].color = this->particles[i]->color;
			//std::cout << this->particles[i]->color.x << " " << this->particles[i]->color.y << " " << this->particles[i]->color.z << std::endl;
		}
	}
	this->SSBOParticles->release();
}

void SPH::render(Scene *scene)
{
	this->shader->useProgram();
	//Enable culling triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//BUFFERS
	for (int i = 0; i < 4; i++)
	{
		//bind UBO buffer light
		glBindBufferBase(GL_UNIFORM_BUFFER, i, scene->getCollector()->getLightUBO(i)->getID());
		//bind UBO lighting with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_lights[i], i);
	}
	glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->SSBOParticles->getID());
	//bind UBO buffer camera
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, scene->getCamUBO()->getID());
	//bind UBO camera with program shader
	glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 0);
	sphere->render(this->nbParticles);
	this->SSBOParticles->release();
	//this->updateSystem();
}