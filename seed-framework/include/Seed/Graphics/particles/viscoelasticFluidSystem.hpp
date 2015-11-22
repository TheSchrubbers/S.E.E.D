/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file viscoelasticFluidSystem.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef VISCOELASTICFLUIDSYSTEM_HPP
#define VISCOELASTICFLUIDSYSTEM_HPP

#include <Seed/Graphics/particles/particlesSystem.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class Scene;

/*! \class ParticlesWaterSystem
*/
class ViscoElasticFluidSystem : public ParticlesSystem
{
public:
	/*
	* \brief Constructor of ParticlesWaterSystem. Construct a particles system which simulates water
	* \param scene : The current scene
	* \param number : The number of particles
	* \param shapeStarter : The shape of the starter : SEED_SPHERE, SEED_POINT, SEED_CIRCLE, SEED_SQUARE
	*/
	ViscoElasticFluidSystem(Scene *scene, const int number, const unsigned int shapeStarter, const glm::vec3 positionStarter);
	~ViscoElasticFluidSystem();
	void print();
private:

	struct Particle
	{
		glm::vec4 position;
		glm::vec4 velocity;
		glm::vec4 life;
		glm::mat4 M;
	};


	void loadSystem();
	void createSystem();
	void updateSystem();
	void simulation();
	void applyViscosity(Particle &p);

	std::vector<Particle> particles;

	Scene *scene;
	glm::vec3 positionStarter;
	unsigned int shapeStarter;
};


#endif