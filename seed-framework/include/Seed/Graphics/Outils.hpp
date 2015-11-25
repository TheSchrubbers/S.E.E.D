#ifndef OUTILS_HPP
#define OUTILS_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <Seed/Graphics/Constant.hpp>
#include <Seed/Graphics/particles/SPH.hpp>

struct Point{
	glm::vec3 p;
	glm::vec3 n;
	float d;
};

void printMat4(glm::mat4 & mat);
void printVec4(glm::vec4 & vec);
void printErrorOpenGL();
void scanSeedError(unsigned int flag);
bool pointIntoSphere(glm::vec3 &p, glm::vec3 &center, float &radius);
bool intersectionSpherePlan(glm::vec3 &center, float &r, glm::vec3 &N, glm::vec3 &P);
/*!
* \brief Function which returns true if the sphere intersect the plan
* \param C The center of the sphere
* \param P The point on the plan
* \return true if the sphere intersect plan
*/
bool intersectionSphereAlignedPlan(glm::vec3 &C, float r, glm::vec3 &P, int k);

/*!
* \brief Function which returns true if the sphere intersect the plan
* \param C The center of the sphere
* \param P The point on the plan
* \return true if the sphere intersect plan
*/
bool intersectionSphereAlignedPlan(glm::vec4 &C, float r, glm::vec3 &P, int k);

//sort vector
struct sortPointsX
{
	bool operator()(const ParticleSPH* p1, const ParticleSPH* p2) const {
		return p1->position.x < p2->position.x;
	}
};
struct sortPointsY
{
	bool operator()(const ParticleSPH* p1, const ParticleSPH* p2) const {
		return p1->position.y < p2->position.y;
	}
};
struct sortPointsZ
{
	bool operator()(const ParticleSPH* p1, const ParticleSPH* p2) const {
		return p1->position.z < p2->position.z;
	}
};

void translate(glm::mat4 &M, const glm::vec3 &T);
void scale(glm::mat4 &M, const glm::vec3 &K);


#endif
