#ifndef CONTROL_HPP
#define CONTROL_HPP

//OTHER INCLUDES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <AntTweakBar.h>

//SEED INCLUDES
#include <Seed/Graphics/engine/Constant.hpp>

class Camera;
class Controller
{
public:
	Controller(GLFWwindow *window);
	~Controller();
	void updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV);
	void updateControl(GLFWwindow* window, Camera* camera, float deltaTime);
	void initAntWeakBar(std::string name);

	TwBar* getBar();
	static int context;
private:
	TwBar *bar;
	Camera *cam;
};

void mouse_buttonID_callback(GLFWwindow* window, int button, int action, int mods);


#endif