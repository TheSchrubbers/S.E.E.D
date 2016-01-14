#include <Seed/Graphics/particles/viscoelasticFluidSystem.hpp>
#include <glm/gtc/random.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <Seed/Graphics/shader.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>

ViscoElasticFluidSystem::ViscoElasticFluidSystem(Scene *sce, const int nb, const unsigned int sS, const glm::vec3 pS)
{
	this->deltaT;
	this->nbParticles = nb;
	this->positionStarter = pS;
	this->shapeStarter = sS;
	this->scene = sce;
	this->createSystem();
	this->loadSystem();
}
ViscoElasticFluidSystem::~ViscoElasticFluidSystem()
{
	delete this->SSBOParticles;
}

void ViscoElasticFluidSystem::loadSystem()
{
	this->SSBOParticles->updateBuffer((void*)(&this->particles[0]), this->nbParticles * sizeof(Particle));
	
}
void ViscoElasticFluidSystem::createSystem()
{
	Particle p;
	for (int i = 0; i < this->nbParticles; i++)
	{
		p.position = glm::vec4(positionStarter,0.0);
		p.velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
		p.life = glm::vec4(1.0, 0.0, 0.0, 0.0);
		p.M = glm::mat4(1.0);
		particles.push_back(p);
	}
	
	//SSBO creating
	//new SSBObuffer
	this->SSBOParticles = new SSBOBuffer();
	//create a SSBOBuffer with a size (number of particles * size of the strcut of a particle
	this->SSBOParticles->createBuffer(this->nbParticles * sizeof(Particle));

}

void ViscoElasticFluidSystem::updateSystem()
{
	this->SSBOParticles->bind();
	//get the SSBO data
	Particle *p = (Particle*)this->SSBOParticles->getData(sizeof(Particle) * this->nbParticles);
	//update the SSBO
	for (int i = 0; i < this->nbParticles; i++)
	{
		p[i].position = glm::vec4(positionStarter, 0.0);
		p[i].velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
		p[i].life = glm::vec4(1.0, 0.0, 0.0, 0.0);
		p[i].M = glm::mat4(1.0);
	}
	this->SSBOParticles->release();

}

void ViscoElasticFluidSystem::simulation()
{
	for (Particle p : particles)
	{
		//p.velocity += this->deltaT * seed_gravity;

		this->applyViscosity(p);
	}
}

void ViscoElasticFluidSystem::applyViscosity(Particle &p)
{

}

void ViscoElasticFluidSystem::print()
{

}