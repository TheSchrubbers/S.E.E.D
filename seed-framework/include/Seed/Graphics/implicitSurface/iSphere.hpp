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
* \file camera.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef ISPHERE_HPP
#define ISPHERE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Seed/Graphics/marchingCubes/marchingCubes.hpp>

/*! \class ImplicitSphere
* \brief ImplicitSphere class 
*/
class ImplicitSphere
{
public:
	/*!
	* \brief Constructor of ImplicitSphere class : X*X + Y*Y + Z*Z - R*R = 0
	* \param radius Radius of the implicit sphere
	* \param position Position of the sphere
	*/
	ImplicitSphere(float radius, glm::vec3 position);
	/*!
	* \brief Destructor of ImplicitSphere class
	*/
	~ImplicitSphere();
	/*!
	* \brief Process the density of a vertice
	* \param start A vertice
	* \param density The result density (pointer on an empty value)
	*/
	void density(glm::vec3 vertice, float *density);
	/*!
	* \brief Process the density of a range of vertices beginning by a starting vertice
	* \param start Starting vertice of the range of vertices
	* \param dz The stripe between each point
	* \param numPoints Size of the range of vertices
	* \param points The range of  vertices
	* \param offset Offset which the range of vertices will be process
	*/
	void density(glm::vec3 start, float dz, int numPoints, std::vector<std::shared_ptr<Point>> &points, int offset);
	/*!
	* \brief Compute the bounding box of the implicit sphere
	* \param V1 The min vertice of the bounding box (pointer on an empty value)
	* \param V2 The max vertice of the bounding box (pointer on an empty value)
	*/
	void getBoundingBox(glm::vec3 *V1, glm::vec3 *V2);
	/*!
	* \brief Compute gradient of the function from a point on the surface
	* \param position position of the point which the gradient is computed
	* \return normal of the point
	*/
	glm::vec3 getGradient(glm::vec3 position);

private:
	float radius, sqrt_radius;
	glm::vec3 position, V1, V2;
	glm::mat4 M; 
};

#endif