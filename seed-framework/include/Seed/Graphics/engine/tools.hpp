#ifndef TOOLS_HPP
#define TOOLS_HPP

//SEED INCLUDES
#include <Seed/Graphics/engine/Constant.hpp>
#include <Seed/Graphics/particles/SPH.hpp>

//OTHER INCLUDES
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <GL/glew.h>

//DEFINES
#define PAUSE system("pause");
#define TEST std::cout << "test " << std::endl;

struct Point{
	glm::vec3 p;
	glm::vec3 n;
	float d;
};

/**
 * @brief      Print matrix 4*4.
 *
 * @param      mat   Matrix4*4 to print
 */
void printMat4(glm::mat4 & mat);
/**
 * @brief      Print vector 4
 *
 * @param      vec   Vector4 to print
 */
void printVec4(glm::vec4 & vec);
/**
 * @brief      Print vector 3.
 *
 * @param      vec   Vector3 to print
 */
void printVec3(glm::vec3 & vec);
/**
 * @brief      Print errors openGL on error's stack.
 */
void printErrorOpenGL();
/**
 * @brief      Print corresponding error flag.
 *
 * @param  flag  flag's error
 */
void scanSeedError(unsigned int flag);
/**
 * @brief      Give a random point into a given sphere.
 *
 * @param      p       Random point
 * @param      center  Center of the given sphere
 * @param      radius  Radius of the given sphere
 *
 * @return     return true if the random point is found
 */
bool pointIntoSphere(glm::vec3 &p, glm::vec3 &center, float &radius);
/**
 * @brief      Test if an intersection between the given sphere and the given plan.
 *
 * @param      center  Center of the given sphere
 * @param      r       Radius of the given sphere
 * @param      N       Normal of the given plan
 * @param      P       A point on the given plan
 *
 * @return     Return true if intersection exists
 */
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

/**
 * @brief      Modify matrix with translation vector
 *
 * @param  M     Matrix to modify
 * @param  T     Translation vector3
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 translate(const glm::mat4 &M, const glm::vec3 &T);
/**
 * @brief      Modify matrix with translation vector
 *
 * @param  M     Matrix to modify
 * @param  T     Translation vector4
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 translate(const glm::mat4 &M, const glm::vec4 &T);
/**
 * @brief      Modify matrix with scale vector
 *
 * @param  M     Matrix to modify
 * @param  T     Scale vector3
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 scale(const glm::mat4 &M, const glm::vec3 &K);
/**
 * @brief      Modify matrix with scale vector
 *
 * @param  M     Matrix to modify
 * @param  T     Scale vector
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 scale(const glm::mat4 &M, const glm::vec4 &K);
/**
 * @brief      Modify matrix with rotation vector
 *
 * @param  M     Matrix to modify
 * @param  T     Rotation vector3
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 rotate(const glm::mat4 &M, const glm::vec3 &K);
/**
 * @brief      Modify matrix with rotation vector
 *
 * @param  M     Matrix to modify
 * @param  T     Rotation vector4
 *
 * @return     Matrix 4*4 modified
 */
glm::mat4 rotate(const glm::mat4 &M, const glm::vec4 &K);
/**
 * @brief      Norm of the vector
 *
 * @param  A     Vector3
 *
 * @return     Norm of the vector
 */
float norm(const glm::vec3 &A);
/**
 * @brief      Norm of the vector
 *
 * @param  A     Vector4
 *
 * @return     Norm of the vector
 */
float norm(const glm::vec4 &A);


#endif
