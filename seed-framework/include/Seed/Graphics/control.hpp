#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <AntTweakBar.h>
#include <Seed/Graphics/camera.hpp>


class Controller
{
public:
	Controller(GLFWwindow *window);
	~Controller();
	void updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV);
	void updateControl(GLFWwindow* window, Camera* camera, float deltaTime);
private:
	int context;
};

void mouse_buttonID_callback(GLFWwindow* window, int button, int action, int mods);


#endif