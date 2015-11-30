#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/data_structure/KDtree.hpp>

#define DELTA 3.0
#define ALPHA 0.1

SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
	this->nbParticles = nb;
	//set the starter of the particles
	this->starter = new Starter();
	//get the shader's particles
	//create system
	this->createSystem(radius, Raffect, sce);
	//init distances surface
	/*for (ParticleSPH *p : this->particles)
	{
		std::vector<ParticleSPH*> parts = this->kdtree->radiusNeighbouring(p, Raffect);
		glm::vec3 positionAverage(0.0);
		for (ParticleSPH* pp : parts)
		{
			positionAverage += glm::vec3(pp->position);
		}
		positionAverage /= (float)parts.size();
		p->parameters.z = 1.0 / glm::distance(positionAverage, glm::vec3(p->position));
	}*/
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
		p->position = glm::vec4(pos[i], 1);
		//transformation matrix of the particle i
		p->M = glm::mat4(1.0);
		//translation of the particle i
		p->M = translate(p->M, glm::vec3(pos[i].x, pos[i].y, pos[i].z));
		//scale of the particle i
		p->M = scale(p->M, glm::vec3(r));
		//inverse transformation matrix of the particle i
		p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(sce->getCamera()->getViewMatrix(), p->M)));
		//color of the particle i
		p->color = glm::vec4(1.0);
		//parameters
		p->parameters = glm::vec4(0.0, r, 0.0, rA);
		//we push the particle i into the array of particles
		particles.push_back(p);
	}

	//create KDTree for neighbouring particles
	this->kdtree = new KDtree(particles, 1000);
	this->processRadiusEffect();
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
	//this->SSBOParticles->updateBuffer(&v[0], this->nbParticles * sizeof(ParticleSPH));
}

void SPH::updateSystem()
{
	std::cout << this->particles.size() << std::endl;
	this->SSBOParticles->deleteBuffer();
	this->SSBOParticles->createBuffer(this->particles.size() * sizeof(ParticleSPH));
	std::vector<ParticleSPH> v;
	for (ParticleSPH *pp : this->particles)
	{
		v.push_back(*pp);
	}
	this->SSBOParticles->updateBuffer(&v[0], this->particles.size() * sizeof(ParticleSPH));
	/*this->SSBOParticles->bind();
	//get the SSBO data
	ParticleSPH *p = (ParticleSPH*)this->SSBOParticles->getData(sizeof(ParticleSPH) * this->nbParticles);
	if (p)
	{
		//update the SSBO
		for (int i = 0; i < this->nbParticles; i++)
		{
			p[i].M = this->particles[i]->M;
			p[i].color = this->particles[i]->color;
			p[i].position = this->particles[i]->position;
			p[i].parameters = this->particles[i]->parameters;
		}
	}
	this->SSBOParticles->release();*/
}

void SPH::updateSystem(int i)
{
	this->SSBOParticles->bind();
	//get the SSBO data
	ParticleSPH *p = (ParticleSPH*)this->SSBOParticles->getData(sizeof(ParticleSPH) * this->nbParticles);
	if (p)
	{
		p[i].M = this->particles[i]->M;
		p[i].color = this->particles[i]->color;
		p[i].position = this->particles[i]->position;
		p[i].parameters = this->particles[i]->parameters;
	}
	this->SSBOParticles->release();
}

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
		std::vector<ParticleSPH*> parts = this-> kdtree->radiusNeighbouring(p, p->parameters.w);
		glm::vec3 positionAverage(0.0);
		for (ParticleSPH* pp : parts)
		{
			positionAverage += glm::vec3(pp->position);
		}
		positionAverage /= (float)parts.size();
		p->parameters.z = 1.0 / glm::distance(positionAverage, glm::vec3(p->position));
	}
}

void SPH::merge(ParticleSPH *p)
{
	float rA = p->parameters.y * 3.0;
	std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p, rA);
	for (ParticleSPH *neighbourp : neighbors)
	{
		if (neighbourp->parameters.x == p->parameters.x)
		{
			p->parameters.x += 1.0;
			p->position = (p->position + neighbourp->position) / glm::vec4(2.0);
			p->M = translate(glm::mat4(1.0), p->position);
			p->M = scale(p->M, glm::vec3(2.0 * p->parameters.y));
			if (p->parameters.x == 1)
				p->color = glm::vec4(1.0, 0.0, 1.0, 1.0);
			else if (p->parameters.x == 2)
				p->color = glm::vec4(0.0, 1.0, 1.0, 1.0);
			else if (p->parameters.x == 3)
				p->color = glm::vec4(0.0, 0.0, 1.0, 1.0);
			p->parameters.y *= 2.0;
			for (std::vector<ParticleSPH*>::iterator i = this->particles.begin(); i != this->particles.end(); i++)
			{
				if (*i == neighbourp)
				{
					this->particles.erase(i);
					break;
				}
			}
			break;
		}
	}
}

void SPH::split(ParticleSPH *p)
{
	float rA = p->parameters.y * 2.0;
	std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p, rA);
	for (ParticleSPH *neighbourp : neighbors)
	{
		p->parameters.x += 1;
		p->position = (p->position + neighbourp->position) / glm::vec4(2.0);
		//printMat4(p->M);
		p->M = translate(glm::mat4(1.0), p->position);
		p->M = scale(p->M, glm::vec3(2.0 * p->parameters.y));
		if (p->parameters.x == 1)
			p->color = glm::vec4(1.0, 0.0, 1.0, 1.0);
		else if (p->parameters.x == 2)
			p->color = glm::vec4(0.0, 1.0, 1.0, 1.0);
		else if (p->parameters.x == 3)
			p->color = glm::vec4(1.0, 0.0, 1.0, 1.0);
		break;
	}
}

void SPH::algorithm()
{
	//process radius effect
	this->processRadiusEffect();
	//merge & split
	for (ParticleSPH *ptmp : this->particles)
	{
		if (ptmp->parameters.z > DELTA)
		{
			this->merge(ptmp);
		}
		else if (ptmp->parameters.z < ALPHA)
		{
			this->split(ptmp);
		}
	}
	//update ssbo
	this->updateSystem();
}

void SPH::processRadiusEffect()
{
	//for each particle, process the distance to the surface
	for (ParticleSPH *p : this->particles)
	{
		std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p, p->parameters.w);
		glm::vec3 positionAverage(0.0);
		for (ParticleSPH* neighbour : neighbors)
		{
			positionAverage += glm::vec3(neighbour->parameters.x + 1.01) * glm::vec3(neighbour->position);
		}
		positionAverage /= (float)neighbors.size();
		p->parameters.z = p->parameters.y / glm::distance(positionAverage, glm::vec3(p->position));
	}
}

glm::vec3 SPH::weight(ParticleSPH *p, ParticleSPH *pNeighbour)
{
	return glm::pow(glm::vec3(1.0) - glm::pow(glm::abs(glm::vec3(p->position - pNeighbour->position)), glm::vec3(2.0)), glm::vec3(3.0));
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