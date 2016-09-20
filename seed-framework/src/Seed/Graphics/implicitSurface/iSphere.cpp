#include <Seed/Graphics/implicitSurface/iSphere.hpp>
#include <iostream>
#define EPSILON 0.001

ImplicitSphere::ImplicitSphere(float r, glm::vec3 pos)
{
	this->radius = r;
	//the square radius of the sphere
	this->sqrt_radius = r*r;
	//the position of the sphere
	this->position = pos;
	// the bounding vertices of the bounding box
	this->V1 = this->position + glm::vec3(r);
	this->V2 = this->position - glm::vec3(r);
}

ImplicitSphere::~ImplicitSphere(){}

void ImplicitSphere::density(glm::vec3 start, float *densities)
{
	float sqrt_r, sqrt_xyz;
	//process density of the vertice
	*densities = start.x * start.x + start.y * start.y + start.z * start.z - this->sqrt_radius;
}

void ImplicitSphere::density(glm::vec3 start, float dz, int numPoints, std::vector<std::shared_ptr<Point>> &points, int offset)
{
	float sqrt_r, tmp_xyr = start.x * start.x + start.y * start.y - this->sqrt_radius;
	for (int i = offset; i < numPoints + offset; i++)
	{
		//apply matrix tranformation
		points[i]->d = start.z * start.z + tmp_xyr;
		start.z += dz;
	}
}

void ImplicitSphere::getBoundingBox(glm::vec3 *V11, glm::vec3 *V22)
{
	*V11 = this->V1;
	*V22 = this->V2;
}

glm::vec3 ImplicitSphere::getGradient(glm::vec3 position)
{
	float f = position.x * position.x + position.y * position.y + position.z * position.z - this->sqrt_radius;
	return glm::normalize(glm::vec3(
		((position.x + EPSILON) * (position.x + EPSILON) + position.y * position.y + position.z * position.z - sqrt_radius) - f,
		(position.x * position.x + (position.y + EPSILON) * (position.y + EPSILON) + position.z * position.z - sqrt_radius) - f,
		(position.x * position.x + position.y * position.y + (position.z + EPSILON) * (position.z + EPSILON) - sqrt_radius) - f
		));
}