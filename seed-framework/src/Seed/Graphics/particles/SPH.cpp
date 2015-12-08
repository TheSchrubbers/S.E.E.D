#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/data_structure/KDtree.hpp>
#include <glm/gtc/random.hpp>
#include <Seed/Graphics/Outils.hpp>

//#define DELTA 0.3f
#define ALPHA 0.04f
#define WATER_DENSITY 1.0f

SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
	this->nbParticles = nb;
	this->scene = sce;
	//set the starter of the particles
	this->starter = new Starter();
	//get the shader's particles
	//create system
	this->createSystem(radius, Raffect);
}

SPH::~SPH()
{
	delete this->starter;
	kd_free(this->kdtree);
	for (ParticleSPH *p : particles)
	{
		delete p;
	}
}

void SPH::print()
{
}

void SPH::loadSystem()
{
	this->SSBOParticles->updateBuffer((void*)(&this->particles[0]), this->nbParticles * sizeof(ParticleSPH));
}

void SPH::createSystem(float r, float rA)
{
	//starter of particles in sphere
	std::vector<glm::vec3> pos = this->starter->addSphereStarter(glm::vec3(0.0), 0.5, this->nbParticles);
	//std::vector<glm::vec3> pos = this->starter->addCubeStarter(glm::vec3(0.0), rA, this->nbParticles);
	//for each particle we set the parameters
	for (int i = 0; i < this->nbParticles; i++)
	{
		ParticleSPH *p = new ParticleSPH;
		//position of the particle i
		p->position = glm::vec4(pos[i], 1);
		p->velocity = glm::vec4(0.0);
		//transformation matrix of the particle i
		p->M = glm::mat4(1.0);
		//translation of the particle i
		p->M = translate(p->M, glm::vec3(pos[i].x, pos[i].y, pos[i].z));
		//scale of the particle i
		p->M = scale(p->M, glm::vec3(r));
		//inverse transformation matrix of the particle i
		p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(this->scene->getCamera()->getViewMatrix(), p->M)));
		//color of the particle i
		p->color = glm::vec4(1.0);
		//parameters
		p->parameters = glm::vec4(0.0, r, 0.0, rA);
		//mass
		p->parameters2.x = Scene::mass;
		//we push the particle i into the array of particles
		particles.push_back(p);
	}

	//create KDTree for neighbouring particles
	//this->kdtree = new KDtree(particles, 1000);
	this->kdtree = kd_create(3);
	for (ParticleSPH *p : this->particles)
	{
		kd_insert3f(this->kdtree, p->position.x, p->position.y, p->position.z, p);
	}

	int o = 3;
	kdres *neighbors = kd_nearest_range3f(this->kdtree, particles[o]->position.x, particles[o]->position.y, particles[o]->position.z, particles[o]->parameters.w);
	ParticleSPH* neighbour;
	while (!kd_res_end(neighbors))
	{
		neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
		neighbour->color = glm::vec4(1.0, 0.0, 0.0, 1.0);
		kd_res_next(neighbors);
	}
	particles[o]->color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	kd_res_free(neighbors);

	this->processRadiusEffect();
	//SSBO creating
	//new SSBObuffer
	this->SSBOParticles = new SSBOBuffer();
	//create a SSBOBuffer with a size (number of particles * size of the strcut of a particle)
	this->SSBOParticles->createBuffer(this->nbParticles * sizeof(ParticleSPH));
	//setting particles into the ssbo buffer
	//this->SSBOParticles->updateBuffer(&v[0], this->nbParticles * sizeof(ParticleSPH));
}

void SPH::updateSystem()
{
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
}

int SPH::getNbParticles()
{
	return this->nbParticles;
}

GLuint SPH::getSSBOID()
{
	return this->SSBOParticles->getID();
}

void SPH::merge(ParticleSPH *p)
{
	//std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p->position, p->parameters.w);
	/*kd_nearest_range3f(this->kdtree, p->)
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
	}*/
}

void SPH::split(ParticleSPH *p)
{
	//for (int i = 0; i < 5; i++)
	/*{
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
	}*/
}

void SPH::algorithm()
{
	//process radius effect
	this->processRadiusEffect();
	//process all forces 
	this->processForces();
	this->collision();
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
	//update matrix
	this->updateMatrix();
	//update ssbo
	this->updateSystem();

	//update particles
	kd_clear(this->kdtree);

	for (ParticleSPH *p : this->particles)
	{
		kd_insert3f(this->kdtree, p->position.x, p->position.y, p->position.z, p);
	}

}

void SPH::processRadiusEffect()
{
	kdres *neighbors = new kdres;
	ParticleSPH* neighbour;
	//for each particle, process the distance to the surface
	for (ParticleSPH *p : this->particles)
	{
		glm::vec3 positionAverage(0.0);
		//std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p, p->parameters.w);
		/*for (ParticleSPH* neighbour : neighbors)
		{
		positionAverage += glm::vec3(neighbour->parameters.x + 1.01) * glm::vec3(neighbour->position);
		}*/
		//positionAverage /= (float)neighbors.size();
		neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
		while (!kd_res_end(neighbors))
		{
			neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
			if (neighbour != p)
			{
				positionAverage += glm::vec3(neighbour->parameters.x + 1.01) * glm::vec3(neighbour->position);
			}
			kd_res_next(neighbors);
		}
		kd_res_free(neighbors);
		positionAverage /= (float)kd_res_size(neighbors);
		p->parameters.z = glm::distance(positionAverage, glm::vec3(p->position)) / p->parameters.w;
	}
}

void SPH::processForces()
{
	glm::vec4 seed_gravity(0.0, -9.81, 0.0, 1.0);
	float dist = 0.0f, hi, hi6, hj, hj6;
	float tmp = 15.0f / (SEED_PI);
	float tmp2 = -45.0f / (SEED_PI);
	float tmp3 = 45.0f / (SEED_PI);
	ParticleSPH* neighbour;
	glm::vec4 Fpressure, Fviscosity, Fgravity,  q1, q2, W, gradientW1, gradientW2, dW, q, Pi, Pj, Vi, Vj, laplacianW1, laplacianW2, vector;
	kdres *neighbors = new kdres;
	//process smoothing kernel, density and pressure of each particle
	for (ParticleSPH *p : this->particles)
	{
		//preprocess value
		hi = p->parameters.w;
		hi6 = glm::pow(p->parameters.w, 6.0f);
		//p->parameters2.y = 0;
		p->density = glm::vec4(0.0f);
		//neighbors of the particle i
		/*std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p->position, hi);
		for (ParticleSPH* neighbour : neighbors)
		{	
			//distance between the particle i and the particle j
			dist = glm::distance(neighbour->position, p->position);
			//process smoothing kernel
			W = glm::vec4(tmp / hi6) * glm::pow((glm::vec4(hi) - dist), glm::vec4(3.0f));
			//process density of the particle i
			//density = sum(mass_j * smoothing_kernel);
			p->density += neighbour->parameters2.x * W;
		}*/
		neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
		while (!kd_res_end(neighbors))
		{
			neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
			if (neighbour != p)
			{
				//distance between the particle i and the particle j
				dist = glm::distance(neighbour->position, p->position);
				//process smoothing kernel
				W = glm::vec4(tmp / hi6) * glm::pow((glm::vec4(hi) - dist), glm::vec4(3.0f));
				//process density of the particle i
				//density = sum(mass_j * smoothing_kernel);
				p->density += neighbour->parameters2.x * W;
			}
			kd_res_next(neighbors);
		}
		kd_res_free(neighbors);
		p->pression = Scene::K * (WATER_DENSITY - p->density);
	}

	for (ParticleSPH *p : this->particles)
	{
		//init
		Fpressure = glm::vec4(0.0);
		Fviscosity = glm::vec4(0.0);
		//preprocess values
		hi = p->parameters.w;
		hi6 = glm::pow(p->parameters.w, 6.0f);
		//process pressure of the particle i
		/*std::vector<ParticleSPH*> neighbors = this->kdtree->radiusNeighbouring(p->position, p->parameters.w);
		std::cout << neighbors.size() << std::endl;
		for (ParticleSPH* neighbour : neighbors)
		{
			hj = neighbour->parameters.w;
			hj6 = glm::pow(neighbour->parameters.w, 6.0f);
			//distance between the particle i and its neighbour j
			dist = glm::distance(p->position, neighbour->position);
			//process gradient of smoothing kernel of Kelager
			gradientW1 = (tmp2 / hi6) * ((neighbour->position - p->position) / dist) * (hi - (dist * dist));
			//process laplacian of smoothing kernel of kelager
			laplacianW1 = glm::vec4((tmp3 / hi6) * (1.0f / dist) * (hi - dist) * (hi - 2.0f*dist));
			//process Force pressure of the particle i
			Fpressure += ((p->pression + neighbour->pression) / 2.0f) * (neighbour->parameters2.x / neighbour->density) * gradientW1;
			//Fviscosity += (MU*(Vi*Vj)*(neighbour->velocity - p->velocity)*(laplacianW1 + laplacianW2)) / 2.0f;
		}*/
		neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
		while (!kd_res_end(neighbors))
		{
			neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
			if (neighbour != p)
			{
				hj = neighbour->parameters.w;
				hj6 = glm::pow(neighbour->parameters.w, 6.0f);
				//distance between the particle i and its neighbour j
				dist = glm::distance(p->position, neighbour->position);
				//process gradient of smoothing kernel of Kelager
				gradientW1 = (tmp2 / hi6) * ((neighbour->position - p->position) / dist) * (hi - (dist * dist));
				//process laplacian of smoothing kernel of kelager
				//laplacianW1 = glm::vec4((tmp3 / hi6) * (1.0f / dist) * (hi - dist) * (hi - 2.0f*dist));
				laplacianW1 = glm::vec4(tmp3 / hi6) * (hi - dist);
				//process Force pressure of the particle i
				Fpressure += ((p->pression + neighbour->pression) / 2.0f) * (neighbour->parameters2.x / neighbour->density) * gradientW1;
				Fviscosity += (neighbour->velocity - p->velocity) * neighbour->parameters2.x * laplacianW1;
			}
			kd_res_next(neighbors);
		}
		kd_res_free(neighbors);
		Fpressure = -Fpressure;
		Fviscosity *= (Scene::mu / p->density);
		Fgravity = p->density * seed_gravity;
		//p->F = Fgravity;// +Fpressure + Fviscosity;
		p->F = Fgravity + Fpressure + Fviscosity;

	}
	for (ParticleSPH *p : this->particles)
	{
		p->velocity += (Scene::deltat * p->F) / p->parameters2.x;
		p->position += Scene::deltat * p->velocity;
	}
}

void SPH::updateMatrix()
{
	for (ParticleSPH *p : this->particles)
	{
		p->M = translate(p->M, p->position);
		p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(this->scene->getCamera()->getViewMatrix(), p->M)));
	}
}

void SPH::collision()
{
	for (ParticleSPH *p : this->particles)
	{
		if (p->position.y < -3.0f)
		{
			p->velocity.y -= p->velocity.y * 0.01;
			p->position.y = -3.0f;
		}
	}
}