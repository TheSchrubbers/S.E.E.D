#ifndef CAMERA_HPP
#define CAMERA_HPP

#ifdef _WIN32
	#include <glm\glm.hpp>
	#include <glm\gtx\transform.hpp>
#else
	#include <glm/glm.hpp>
	#include <glm/gtx/transform.hpp>
#endif

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 look, glm::vec3 upVector, float FOV, float width, float height, float near, float far);
	~Camera(){};
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void setViewMatrix(glm::vec3 & pos, glm::vec3 &dir, glm::vec3 &up);
	void setProjectionMatrix(const float &Fov, const int width, const int height, const float &near, const float &far);
private:
	glm::mat4 View_matrix;
	glm::mat4 Projection_matrix;
};

#endif