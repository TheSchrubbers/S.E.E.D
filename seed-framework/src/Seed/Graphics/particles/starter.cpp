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
std::vector<glm::vec3> Starter::addCubeStarter(glm::vec3 pos, float r, int nb)
{
	int n = glm::sqrt(nb);
	n = glm::sqrt(n);
	std::vector<glm::vec3> positions;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				positions.push_back(glm::vec3(i * r, j * r, k * r));
			}
		}
	}
	return positions;
}