#include <Seed/Graphics/particles/starter.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

Starter::Starter()
{

}

Starter::~Starter()
{

}

std::vector<glm::vec3> Starter::addSphereStarter(glm::vec3 pos, float r,  int nb)
{
	std::vector<glm::vec3> positions;
	for (int i = 0; i < nb; i++)
	{
		positions.push_back(glm::ballRand(r) + pos);
	}
	return positions;
}