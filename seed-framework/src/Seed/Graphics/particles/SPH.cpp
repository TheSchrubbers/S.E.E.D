#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/particles/starter.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <Seed/Graphics/Outils.hpp>
#include <math.h>

#define DELTA 0.3f
#define ALPHA 0.04f
#define PRESSION 101325
#define BOUNDPRESSION 0.1
#define H 10
SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
	this->nbParticles = nb;
	this->scene = sce;
	//set the starter of the particles
	this->starter = new Starter();
	//get the shader's particles
	//create system
	this->createSystem(radius, Raffect);
	ppp = nullptr;
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
	diffT = 0;
	//starter of particles in sphere
	//std::vector<glm::vec3> pos = this->starter->addSphereStarter(glm::vec3(0.0), Scene::radiusSphereStarter, this->nbParticles);
	std::vector<glm::vec3> pos;
	pos.push_back(glm::vec3(0.0, 0.95, 0.0));
	//std::vector<glm::vec3> pos = this->starter->addCubeStarter(glm::vec3(0.0), Scene::radiusParticle, this->nbParticles);
	rInit = r;
	float volume = (4.0f * SEED_PI * glm::pow(Scene::radiusSphereStarter, 3.0f)) / 3.0f;
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
		Scene::radiusNeighbouring = glm::sqrt(glm::sqrt(glm::sqrt((3.0f * volume * Scene::x) / (4.0f * SEED_PI * this->nbParticles))));
		p->parameters = glm::vec4(0.0, r, 0.0, Scene::radiusNeighbouring);
		//mass
		p->parameters2.x = (Scene::densityFluid * volume) / this->nbParticles;
		p->flag = true;
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

	this->processRadiusEffect();
	//SSBO creating
	//new SSBObuffer
	this->SSBOParticles = new SSBOBuffer();

	Scene::nbPart = this->nbParticles;
}


void SPH::algorithm()
{

	if (Scene::play)
	{
		if (this->diffT > 0.1)
		{
			this->nbParticles++;
			//add particle
			ParticleSPH *p = new ParticleSPH;
			//position of the particle i
			p->position = glm::vec4(glm::vec3(glm::linearRand(-0.1, 0.1), 0.9, 0.0), glm::linearRand(-0.1, 0.1));
			p->velocity = glm::vec4(0.0);
			//transformation matrix of the particle i
			p->M = glm::mat4(1.0);
			//translation of the particle i
			p->M = translate(p->M, p->position);
			//scale of the particle i
			p->M = scale(p->M, glm::vec3(rInit));
			//inverse transformation matrix of the particle i
			p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(this->scene->getCamera()->getViewMatrix(), p->M)));
			//color of the particle i
			p->color = glm::vec4(1.0);
			//parameters
			//float volume = (4.0f * SEED_PI * glm::pow(Scene::radiusSphereStarter, 3.0f)) / 3.0f;
			Scene::radiusNeighbouring = glm::sqrt(glm::sqrt(glm::sqrt((3.0f * Scene::x) / (4.0f * SEED_PI * this->nbParticles))));
			p->parameters = glm::vec4(0.0, rInit, 0.0, Scene::radiusNeighbouring);
			//mass
			p->parameters2.x = (Scene::densityFluid) / this->nbParticles;
			p->flag = true;
			//we push the particle i into the array of particles
			particles.push_back(p);
			diffT = 0.0;

		}
		diffT += (std::clock() - tNow) / (double)CLOCKS_PER_SEC;
	}
	if (Scene::nextFrame || Scene::play)
	{
		tNow = std::clock();
		//process radius effect
		this->processRadiusEffect();
		//process all forces 
		this->processForces();
		//get collision
		this->collision();
		//merge & split
		for (ParticleSPH *ptmp : this->particles)
		{
			if (ptmp->flag)
			{
				//if adaptative parameter of the particle < merge parameter and level < 2.0
				if (ptmp->parameters.z < Scene::mergeCoef && ptmp->parameters.x < 2.0)
				{
					this->merge(ptmp);
				}
				else if (ptmp->parameters.x > 0.0 && ptmp->parameters.z > Scene::splitCoef)
				{
					this->split(ptmp);
				}
			}
		}
		Scene::nextFrame = false;
		//std::cout << (float)diffT << std::endl;
	}
	
	//update matrix
	this->updateMatrix();
	//update ssbo
	this->updateSystem();
	this->updateParticles();

	//update particles
	/*kd_clear(this->kdtree);

	for (ParticleSPH *p : this->particles)
	{
	if (p->flag)
	kd_insert3f(this->kdtree, p->position.x, p->position.y, p->position.z, p);
	}*/

}

void SPH::processRadiusEffect()
{
	kdres *neighbors = new kdres;
	ParticleSPH* neighbour;
	int nbNeighbors = 0;
	//for each particle, process the distance to the surface
	for (ParticleSPH *p : this->particles)
	{
		if (p->flag)
		{
			glm::vec3 positionAverage(0.0);
			//take all neighbors of the particle 
			neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
			//update nbNeighbors
			nbNeighbors += kd_res_size(neighbors);
			//for each particle
			while (!kd_res_end(neighbors))
			{
				//neighbour of the particle
				neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
				//if the particle exists && the neighbour is'nt the same of the current particle
				if (neighbour->flag && neighbour != p)
				{
					//update average position with neighbouring
					positionAverage += glm::vec3(neighbour->parameters.x + 1.01) * glm::vec3(neighbour->position);
				}
				//next particle
				kd_res_next(neighbors);
			}
			//update number of neighbors of all particles
			Scene::AverageNeighbors = (float)nbNeighbors / (float)nbParticles;
			//free the result
			kd_res_free(neighbors);
			//average of the barycenter position
			positionAverage /= (float)kd_res_size(neighbors);
			//update adaptative sampling parameter
			p->parameters.z = glm::distance(positionAverage, glm::vec3(p->position)) / p->parameters.w;
		}
	}
}

void SPH::processForces()
{
	glm::vec4 seed_gravity(0.0, -9.81, 0.0, 1.0);
	float dist = 0.0f, hi, hi2, hi6, hi8, hi9, hj, hj6, hj9;
	float tmp = 15.0f / (SEED_PI);
	float tmp2 = -45.0f / (SEED_PI);
	float tmp3 = 45.0f / (SEED_PI);
	float tmp5 = 315.0 / (64.0 * SEED_PI);
	float niNorm = 0.0f;
	float W;
	float tmp4;
	ParticleSPH* neighbour;
	glm::vec4 shift = glm::vec4(0.0);
	glm::vec4 Fpressure, Fviscosity, Fgravity, FtensionSurface, q1, q2, gradientW1, gradientW2, dW, q, Pi, Pj, Vi, Vj, laplacianW1, laplacianW2, vector, ni, colorField;
	kdres *neighbors = new kdres;
	//process smoothing kernel, density and pressure of each particle
	for (ParticleSPH *p : this->particles)
	{
		if (p->flag)
		{
			//preprocess value
			//hi = p->parameters.w;
			hi = p->parameters.w;
			hi2 = glm::pow(hi, 2.0f);
			hi6 = glm::pow(hi, 6.0f);
			hi8 = glm::pow(hi, 8.0f);
			hi9 = glm::pow(hi, 9.0f);

			p->density = 0.0f;

			//get the neighbors
			neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);

			while (!kd_res_end(neighbors))
			{
				neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
				if (neighbour->flag && neighbour != p)
				{
					//distance between the particle i and the particle j
					dist = glm::distance(p->position, neighbour->position);
					//process smoothing kernel
					W = tmp / hi6 * glm::pow((hi - dist), 3.0f);
					//process density of the particle i
					p->density += neighbour->parameters2.x * W;
				}
				kd_res_next(neighbors);
			}
			kd_res_free(neighbors);
			p->pression = Scene::K * (Scene::densityFluid - p->density);
			
		}
	}
	for (ParticleSPH *p : this->particles)
	{
		//init
		Fpressure = glm::vec4(0.0);
		Fviscosity = glm::vec4(0.0);
		//preprocess values
		hi = p->parameters.w;
		hi6 = glm::pow(hi, 6.0f);
		neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
		while (!kd_res_end(neighbors))
		{
			neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
			if (neighbour->flag && neighbour != p)
			{
				//hj = neighbour->parameters.w;
				//hj6 = glm::pow(neighbour->parameters.w, 6.0f);
				//distance between the particle i and its neighbour j
				dist = glm::distance(p->position, neighbour->position);
				//process gradient of smoothing kernel of Kelager
				gradientW1 = (tmp2 / hi6) * ((p->position - neighbour->position) / dist) * (hi - (dist * dist));
				//process laplacian of smoothing kernel of kelager
				//laplacianW1 = glm::vec4((tmp3 / hi6) * (1.0f / dist) * (hi - dist) * (hi - 2.0f*dist));
				//laplacianW1 = glm::vec4(tmp3 / hi6) * (hi - dist);

				//tmp4 = neighbour->parameters2.x / neighbour->density;
				//surface tension
				//ni += tmp4 * gradientW1;
				//color field
				//colorField += tmp4 * laplacianW1;
				//process internel forces of the particle i
				//pressure force
				//Fpressure += ((p->pression + neighbour->pression) / 2.0f) * (neighbour->parameters2.x / neighbour->density) * gradientW1;
				if (p->density != 0.0 && neighbour->density != 0.0)
				{
					Fpressure += glm::vec4((p->pression / glm::pow(p->density, 2.0)) + (neighbour->pression / glm::pow(neighbour->density, 2.0))) * neighbour->parameters2.x * gradientW1;
					//Fpressure += neighbour->pression * (neighbour->parameters2.x / neighbour->density) * gradientW1;
				}
				//viscosity force
				//Fviscosity += (neighbour->velocity - p->velocity) * neighbour->parameters2.x * laplacianW1;
				//Fviscosity += (neighbour->velocity - p->velocity) * (neighbour->parameters2.x / neighbour->density) * laplacianW1;
				if (glm::distance(p->position, neighbour->position) < p->parameters.y)
				{
					shift += (neighbour->position - p->position);
				}
			}
			kd_res_next(neighbors);
		}
		//niNorm = norm(ni);
		kd_res_free(neighbors);
		//Fpressure = -Fpressure;
		//Fpressure *= (-p->density);
		//Fviscosity *= (Scene::mu / p->density);
		//Fviscosity *= Scene::mu;
		Fgravity = -glm::vec4(0.0f, 9.81 * p->parameters2.x * 4.0f, 0.0f,1.0f);
		//FtensionSurface = glm::vec4(0.0);
		/*if (niNorm >= Scene::threshold)
		{
			FtensionSurface = -Scene::sigma*colorField * (ni / niNorm);
		}*/
		if (Scene::SPHGravity)
		{
			p->F = Fpressure + Fgravity;
		}
		else
		{
			p->F = Fpressure;
		}
	}
	for (ParticleSPH *p : this->particles)
	{
		if (p->flag)
		{
			p->velocity += Scene::deltat * p->F;
			p->velocity.w = 1.0;
			p->position += Scene::deltat * p->velocity;
			//p->position += shift;
			p->position.w = 1.0;
		}
	}
}

void SPH::collision()
{
	//Cube container
	for (ParticleSPH *p : this->particles)
	{
		if (p->flag)
		{
			for (int i = 0; i < 3; i++)
			{
				if (p->position[i] < -Scene::sizeCube)
				{
					p->velocity[i] = -p->velocity[i] * 0.01;
					p->position[i] = -Scene::sizeCube;
				}
				if (p->position[i] > Scene::sizeCube)
				{
					p->velocity[i] = -p->velocity[i] * 0.01;
					p->position[i] = Scene::sizeCube;
				}
			}
		}
	}
}

void SPH::merge(ParticleSPH *p)
{
	ParticleSPH *neighbour;
	ParticleSPH *p2 = new ParticleSPH;
	kdres *neighbors = kd_nearest_range3f(this->kdtree, p->position.x, p->position.y, p->position.z, p->parameters.w);
	//check all neighbors
	while (!kd_res_end(neighbors))
	{
		//a naighbour
		neighbour = (ParticleSPH*)kd_res_item_data(neighbors);
		//if the neighbour exists && has the same level && is not the same of the current particle
		if (neighbour->flag && neighbour->parameters.x == p->parameters.x && neighbour != p)
		{
			//take the middle position between of the two particles
			glm::vec4 midPos = (p->position + neighbour->position) / glm::vec4(2.0);

			if (kd_res_size(neighbors) == 2);
			{
				//new position
				p2->position = midPos;
				//set the w component to 1.0
				p2->position.w = 1.0;
				//new velocity
				p2->velocity = p->velocity / 1.5f;
				//new level
				p2->parameters.x = p->parameters.x + 1.0;
				//new radius of the particle
				p2->parameters.y = p->parameters.y * 1.5;
				//new radius neighbouring
				p2->parameters.w = p->parameters.w * 1.5f;
				//new mass
				p2->parameters2.x = p->parameters2.x * 1.5f;
				//new transformation matrix
				p2->M = scale(glm::mat4(1.0), glm::vec3(p2->parameters.y));
				//new color
				p2->color = this->colorLevel(p2->parameters.x);
				//particle exists
				p2->flag = true;
				//push back the new particle in the table
				this->particles.push_back(p2);
				//push back the new particle in the kdtree
				kd_insert3f(this->kdtree, p2->position.x, p2->position.y, p2->position.z, p2);
				//desactive the two old particle
				neighbour->flag = false;
				p->flag = false;
				ppp = p2;
				//finish
				break;
			}
		}
		//next particle
		kd_res_next(neighbors);
	}
}

void SPH::split(ParticleSPH *p)
{
	ParticleSPH* p2 = new ParticleSPH;
	ParticleSPH* p3 = new ParticleSPH;
	//new position
	p2->position = glm::vec4(glm::sphericalRand(p->parameters.y), 1.0) + p->position;
	//set w component to 1.0f;
	p2->position.w = 1.0;
	//new velocity
	p2->velocity = p->velocity * 1.5f;
	//new level
	p2->parameters.x = p->parameters.x - 1.0f;
	//new radius for the particle
	p2->parameters.y = p->parameters.y / 1.5f;
	//new radius neighbouring
	p2->parameters.w = p->parameters.w / 1.5f;
	//new mass
	p2->parameters2.x = p->parameters2.x / 1.5f;
	//new transformation matrix
	p2->M = scale(glm::mat4(1.0), glm::vec3(p2->parameters.y));
	//new color
	p2->color = this->colorLevel(p2->parameters.x);
	//particle exists
	p2->flag = true;
	//new posiiton
	p3->position = p2->position + (p->position - p2->position) * 2.0f;
	//new level
	p3->parameters.x = p->parameters.x - 1.0f;
	//new radius of particle
	p3->parameters.y = p->parameters.y / 1.5f;
	//new radius neighbouring
	p3->parameters.w = p->parameters.w / 1.5f;
	//new mass
	p3->parameters2.x = p->parameters2.x / 1.5f;
	//new transformation matrix
	p3->M = scale(glm::mat4(1.0), glm::vec3(p3->parameters.y));
	//new color
	p3->color = this->colorLevel(p2->parameters.x);
	//new velocity
	p3->velocity = p->velocity * 1.5f;
	//particle exists
	p3->flag = true;

	//insert particles in the table and kdtree
	this->particles.push_back(p2);
	this->particles.push_back(p3);
	kd_insert3f(this->kdtree, p2->position.x, p2->position.y, p2->position.z, p2);
	kd_insert3f(this->kdtree, p3->position.x, p3->position.y, p3->position.z, p3);
	//particle not exists
	p->flag = false;
}

//update matrix of each particle
void SPH::updateMatrix()
{
	for (ParticleSPH *p : this->particles)
	{
		if (p->flag)
		{
			p->M = translate(p->M, p->position);
			p->NormalMatrix = glm::transpose(glm::inverse(glm::matrixCompMult(this->scene->getCamera()->getViewMatrix(), p->M)));
		}
	}
	Scene::nbPart = this->nbParticles;
}

void SPH::updateParticles()
{
	/*std::vector<std::vector<ParticleSPH*>::iterator> it;
	std::vector<ParticleSPH*>::iterator i;
	for (i = this->particles.begin(); i != this->particles.end(); i++)
	{
	if (!(*i)->flag)
	{
	it.push_back(i);
	}
	}
	for (int k = 0; k < it.size(); k++)
	{
	this->particles.erase(it[k]);
	}*/
	//update all the kdtree
	kd_clear(this->kdtree);
	for (int k = 0; k < this->particles.size(); k++)
	{
		if (this->particles[k]->flag)
			kd_insert3f(this->kdtree, this->particles[k]->position.x, this->particles[k]->position.y, this->particles[k]->position.z, this->particles[k]);
	}
}

//update all the system
void SPH::updateSystem()
{
	this->nbParticles = 0;
	this->SSBOParticles->deleteBuffer();
	std::vector<ParticleSPHSSBO> v;

	//if rendering half scene
	if (Scene::half)
	{
		//print half scene
		for (ParticleSPH *pp : this->particles)
		{
			if (pp->flag && pp->position.z < 0)
			{
				ParticleSPHSSBO p;
				p.M = pp->M;
				p.color = pp->color;
				p.NormalMatrix = pp->NormalMatrix;
				v.push_back(p);
				this->nbParticles++;
			}
		}
	}
	//rendering all the scene
	else
	{
		for (ParticleSPH *pp : this->particles)
		{
			if (pp->flag)
			{
				ParticleSPHSSBO p;
				p.M = pp->M;
				p.color = pp->color;
				p.NormalMatrix = pp->NormalMatrix;
				v.push_back(p);
				this->nbParticles++;
			}
		}
	}
	//update the SSBObuffer
	this->SSBOParticles->createBuffer(this->nbParticles * sizeof(ParticleSPHSSBO));
	this->SSBOParticles->updateBuffer(&v[0], this->nbParticles * sizeof(ParticleSPHSSBO));
}


//color according to the level of the particle
glm::vec4 SPH::colorLevel(float level)
{
	switch ((int)level)
	{
	case 0:
		return glm::vec4(1.0, 1.0, 1.0, 1.0);
	case 1:
		return glm::vec4(1.0, 0.0, 1.0, 1.0);
	case 2:
		return glm::vec4(0.0, 1.0, 1.0, 1.0);
	case 3:
		return glm::vec4(0.0, 0.0, 1.0, 1.0);
	}
}


int SPH::getNbParticles()
{
	return this->nbParticles;
}

GLuint SPH::getSSBOID()
{
	return this->SSBOParticles->getID();
}

/*SPH::SPH(int nb, float radius, float Raffect, Scene* const sce)
{
this->nbParticles = nb;
this->scene = sce;
//set the starter of the particles
this->starter = new Starter();
//get the shader's particles
//create system
this->createSystem(radius, Raffect);
}*/

/*void SPH::createSystem(float r, float rA)
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
}*/