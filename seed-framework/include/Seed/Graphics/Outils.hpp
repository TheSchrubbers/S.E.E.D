#ifndef OUTILS_HPP
#define OUTILS_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <Seed/Graphics/Constant.hpp>

struct Point{
	glm::vec3 p;
	glm::vec3 n;
	float d;
};

void printMat4(glm::mat4 & mat);
void printVec4(glm::vec4 & vec);
void printErrorOpenGL();
void scanSeedError(unsigned int flag);

//sort vector
struct sortPointsX
{
	bool operator()(const Point* p1, const Point* p2) const {
		return p1->p.x < p2->p.x;
	}
};
struct sortPointsY
{
	bool operator()(const Point* p1, const Point* p2) const {
		return p1->p.y < p2->p.y;
	}
};
struct sortPointsZ
{
	bool operator()(const Point* p1, const Point* p2) const {
		return p1->p.z < p2->p.z;
	}
};

#endif
