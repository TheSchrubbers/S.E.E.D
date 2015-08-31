#ifdef _WIN32
	#include <GLFW\glfw3.h>
	#include <glm\glm.hpp>
#else
	#include <GLFW/glfw3.h>
	#include <glm/glm.hpp>
#endif
#include "Constant.hpp"


void updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV);