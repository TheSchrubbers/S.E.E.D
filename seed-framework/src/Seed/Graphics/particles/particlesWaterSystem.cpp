//SEED INCLUDES
#include <Seed/Graphics/particles/particlesWaterSystem.hpp>
#include <Seed/Graphics/buffers/SSBOBuffer.hpp>
#include <Seed/Graphics/engine/Constant.hpp>
#include <Seed/Graphics/engine/shader.hpp>
#include <Seed/Graphics/engine/scene.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>

//OTHER INCLUDES
#include <glm/gtc/random.hpp>


ParticlesWaterSystem::ParticlesWaterSystem(Scene *sce, const int nb, const unsigned int sS, const glm::vec3 pS)
{
	this->nbParticles = nb;
	this->positionStarter = pS;
	this->shapeStarter = sS;
	this->scene = sce;
	this->createSystem();
	this->loadSystem();
}
ParticlesWaterSystem::~ParticlesWaterSystem()
{
	delete this->SSBOParticles;
}

void ParticlesWaterSystem::loadSystem()
{
	this->SSBOParticles->updateBuffer((void*)(&this->particles[0]), this->nbParticles * sizeof(Particle));
	
}
void ParticlesWaterSystem::createSystem()
{
	Particle p;
	for (int i = 0; i < this->nbParticles; i++)
	{
		p.position = glm::vec4(positionStarter,0.0);
		p.velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
		p.M = glm::mat4(1.0);

		p.level = 0;
		p.r = 0.1;
		particles.push_back(p);
	}
	
	//SSBO creating
	//new SSBObuffer
	this->SSBOParticles = new SSBOBuffer();
	//create a SSBOBuffer with a size (number of particles * size of the strcut of a particle
	this->SSBOParticles->createBuffer(this->nbParticles * sizeof(Particle));

}

void ParticlesWaterSystem::updateSystem()
{
	this->SSBOParticles->bind();
	//get the SSBO data
	Particle *p = (Particle*)this->SSBOParticles->getData(sizeof(Particle) * this->nbParticles);
	//update the SSBO
	for (int i = 0; i < this->nbParticles; i++)
	{
		p[i].position = glm::vec4(positionStarter, 0.0);
		p[i].velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
		p[i].M = glm::mat4(1.0);
	}
	this->SSBOParticles->release();

}

void ParticlesWaterSystem::print()
{

}