#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/data_structure/KDtree.hpp>

SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
	//assimp load sphere
	//Assimp::Importer importer;
	//std::string path = pathToModels + "UVsphereLow.obj";
	//const aiScene *pScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast);
	this->nbParticles = nb;
	//set the starter of the particles
	this->starter = new Starter();
	//get the shader's particles
	//create system
	this->createSystem(radius, Raffect, sce);

	this->zonaradius = Raffect;
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

void SPH::createSystem(float r, float rA, Scene* const sce)
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
		p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(sce->getCamera()->getViewMatrix(), p->M)));
		//color of the particle i
		p->color = glm::vec4(1.0);
		//parameters
		p->parameters = glm::vec4(0.0, r, 0.0, rA);
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

	std::vector<ParticleSPH*> p = this->kdtree->radiusNeighbouring(this->particles[5], rA);
	this->particles[5]->color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	for (int i = 0; i < p.size(); i++)
	{
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
		}
	}
	this->SSBOParticles->release();
}

/*void SPH::render(Scene *scene)
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
}*/

int SPH::getNbParticles()
{
	return this->nbParticles;
}

GLuint SPH::getSSBOID()
{
	return this->SSBOParticles->getID();
}

void SPH::update()
{
	for (ParticleSPH *p : this->particles)
	{
		std::vector<ParticleSPH*> parts = this-> kdtree->radiusNeighbouring(p, this->zonaradius);
		glm::vec3 positionAverage(0.0);
		for (ParticleSPH* pp : parts)
		{
			positionAverage += glm::vec3(pp->position);
		}
		positionAverage /= (float)parts.size();
		p->parameters.z = 1.0 / glm::distance(positionAverage, glm::vec3(p->position));
	}
}