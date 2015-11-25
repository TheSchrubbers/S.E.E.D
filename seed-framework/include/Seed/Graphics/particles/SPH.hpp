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
* \file SSH.hpp
* \author Jérémy RIFFET
* \version 0.1 
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#include <Seed/Graphics/particles/particlesSystem.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include <memory>

#ifndef SPH_HPP
#define SPH_HPP

class Starter;
class InstancedModel;
class Scene;
class Shader;
class KDtree;

struct ParticleSPH
{
	glm::vec4 position;
	glm::mat4 M;
	glm::mat4 NormalMatrix;
	glm::vec4 color;
	//param 1 : level
	//param 2 : radius
	//param 3 : AS (adaptative sampling)
	//param 4 : effect radius surface
	glm::vec4 parameters;
};

/*! \class SPH
*/

class SPH : public ParticlesSystem
{
	public:
		SPH(int nb, float radius, float Raffect, Scene* const sce);
		~SPH();
		void print();
		void update();
		//void render(Scene *scene);

		int getNbParticles();
		GLuint getSSBOID();
	
	protected:

		std::vector<ParticleSPH*> particles;
		Starter *starter;

		void loadSystem();
		void createSystem(float r, float rA, Scene* const sce);
		void updateSystem();
		//float isolevel();
		int nbParticles, type;
		float deltaT;
		float zonaradius;
		KDtree *kdtree;

		glm::mat4 Normal_Matrix;
};


#endif