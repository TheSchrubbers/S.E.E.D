#include <Seed/Graphics/Outils.hpp>
#include <glm/gtx/intersect.hpp>
#include <iostream>

void printMat4(glm::mat4 & mat)
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << mat[i].x << " " << mat[i].y << " " << mat[i].z << " " << mat[i].w << std::endl;
	}
}

void printVec4(glm::vec4 & vec)
{
	std::cout << vec.x << " " <<vec.y << " " << vec.z << " " << vec.w << std::endl;
}

void printVec3(glm::vec3 & vec)
{
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void printErrorOpenGL()
{
	GLenum err;
	std::string error;
	while ((err = glGetError()) != GL_NO_ERROR) {
		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cerr << "OpenGL error: " << error << std::endl;
	}
}

void scanSeedError(unsigned int flag)
{
	switch (flag)
	{
	case 1:
		//std::cout << "SUCCESS" << std::endl;
		break;
	case 50:
		std::cout << "Error, file not founding" << std::endl;
		break;
	case 51:
		std::cout << "Error, default shaders not founding" << std::endl;
		break;
	case 52:
		std::cout << "Error, loading shader" << std::endl;
		break;
	}
}

bool pointIntoSphere(glm::vec3 &p, glm::vec3 &center, float &radius)
{
	if (glm::distance(p, center) <= radius)
		return true;
	return false;
}

bool intersectionSpherePlan(glm::vec3 &center, float &r, glm::vec3 &N, glm::vec3 &P)
{
	float d = -N.x * P.x - N.y * P.y - N.z * P.z;
	float t = (-N.x * center.x - N.y * center.y - N.z * center.z - d) / (N.x * N.x + N.y * N.y + N.z * N.z);
	if (glm::length(N * t) < r)
		return true;
	return false;
}
bool intersectionSphereAlignedPlan(glm::vec3 &C, float r, glm::vec3 &P, int k)
{
	if (glm::abs(C[k] - P[k]) <= r)
		return true;
	return false;
}

bool intersectionSphereAlignedPlan(glm::vec4 &C, float r, glm::vec3 &P, int k)
{
	std::cout << glm::abs(C[k] - P[k]) << std::endl;
	if (glm::abs(C[k] - P[k]) <= r)
	{
		return true;
	}
	return false;
}

glm::mat4 translate(const glm::mat4 &M2, const glm::vec3 &T)
{
	glm::mat4 M = M2;
	for (int i = 0; i < 3; i++)
	{
		M[3][i] = T[i];
	}
	return M;
}

glm::mat4 translate(const glm::mat4 &M2, const glm::vec4 &T)
{
	glm::mat4 M = M2;
	for (int i = 0; i < 3; i++)
	{
		M[3][i] = T[i];
	}
	return M;
}

glm::mat4 scale(const glm::mat4 &M2, const glm::vec3 &K)
{
	glm::mat4 M = M2;
	for (int i = 0; i < 3; i++)
	{
		M[i][i] *= K[i];
	}
	return M;
}

glm::mat4 scale(const glm::mat4 &M2, const glm::vec4 &K)
{
	glm::mat4 M = M2;
	for (int i = 0; i < 3; i++)
	{
		M[i][i] *= K[i];
	}
	return M;
}



