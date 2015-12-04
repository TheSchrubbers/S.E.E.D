#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/data_structure/KDtree.hpp>
#include <glm/gtc/random.hpp>

#define DELTA 0.3
#define ALPHA 0.04
#define DELTAT 0.000001
#define GRAVITY 9.81

SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
	this->nbParticles = nb;
	//set the starter of the particles
	this->starter = new Starter();
	//get the shader's particles
	//create system
	this->createSystem(radius, Raffect, sce);
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
	//std::cout << this->particles.size() << std::endl;
	this->SSBOParticles->deleteBuffer();
	this->SSBOParticles->createBuffer(this->particles.size() * sizeof(ParticleSPHSSBO));
	std::vector<ParticleSPHSSBO> v;
	for (ParticleSPH *pp : this->particles)
	{
		ParticleSPHSSBO p;
		p.M = pp->M;
		p.color = pp->color;
		p.NormalMatrix = pp->NormalMatrix;
		v.push_back(p);
	}
	this->SSBOParticles->updateBuffer(&v[0], this->particles.size() * sizeof(ParticleSPHSSBO));
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
	std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p->position, p->parameters.w);

	for (ParticleSPH *neighbourp : neighbors)
	{
		if (neighbourp->parameters.x == p->parameters.x)
		{
			glm::vec4 midPos = (p->position + neighbourp->position) / glm::vec4(2.0);
			float r = 1.5 * p->parameters.w;// glm::sqrt(glm::sqrt(glm::sqrt(glm::pow(2.0, p->parameters.x)))) * p->parameters.z;
			if (this->kdtree->radiusNeighbouringNumber(midPos, r) == 2);
			{
				p->position = (p->position + neighbourp->position) / glm::vec4(2.0);
				p->parameters = p->parameters;
				p->parameters.x += 1.0;
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
}

void SPH::split(ParticleSPH *p)
{
	//for (int i = 0; i < 5; i++)
	{
		glm::vec4 pos1(glm::sphericalRand(p->parameters.y) + glm::vec3(p->position), 1.0);
		glm::vec4 pos2(pos1 + (p->position - pos1) * glm::vec4(2.0));
		ParticleSPH *p1 = new ParticleSPH;
		p1->position = pos1;
		p->position = pos2;
		glm::vec4 param = glm::vec4(p->parameters.x - 1.0, p->parameters.y / 2.0, p->parameters.z / 2.0, p->parameters.w);
		p1->parameters = param;
		p->parameters = param;
		p1->M = translate(glm::mat4(1.0), p->position);
		p1->M = scale(p->M, glm::vec3(p->parameters.y));
		p->M = translate(glm::mat4(1.0), p->position);
		p->M = scale(p->M, glm::vec3(p->parameters.y));
		if (p1->parameters.x == 1)
			p1->color = glm::vec4(1.0, 0.0, 1.0, 1.0);
		else if (p1->parameters.x == 2)
			p1->color = glm::vec4(0.0, 1.0, 1.0, 1.0);
		else if (p1->parameters.x == 3)
			p1->color = glm::vec4(0.0, 0.0, 1.0, 1.0);
		if (p->parameters.x == 1)
			p->color = glm::vec4(1.0, 0.0, 1.0, 1.0);
		else if (p->parameters.x == 2)
			p->color = glm::vec4(0.0, 1.0, 1.0, 1.0);
		else if (p->parameters.x == 3)
			p->color = glm::vec4(0.0, 0.0, 1.0, 1.0);
		this->particles.push_back(p1);
	}
}

void SPH::algorithm()
{
	//process radius effect
	this->processRadiusEffect();
	this->processForces();
	//merge & split
	/*for (ParticleSPH *ptmp : this->particles)
	{
		//std::cout << ptmp->parameters.z << std::endl;
		if (ptmp->parameters.z < ALPHA && ptmp->parameters.x < 4.0)
		{
			this->merge(ptmp);
		}
		else if (ptmp->parameters.x > 0.0 && ptmp->parameters.z > DELTA)
		{
			//this->split(ptmp);
		}
	}*/
	//PAUSE
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
		p->parameters.z = glm::distance(positionAverage, glm::vec3(p->position)) / p->parameters.w;
	}
}

glm::vec3 SPH::weight(ParticleSPH *p, ParticleSPH *pNeighbour)
{
	return glm::pow(glm::vec3(1.0) - glm::pow(glm::abs(glm::vec3(p->position - pNeighbour->position)), glm::vec3(2.0)), glm::vec3(3.0));
}

void SPH::processForces()
{
	for (ParticleSPH *p : this->particles)
	{
		p->position.y += GRAVITY * DELTAT;
	}
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