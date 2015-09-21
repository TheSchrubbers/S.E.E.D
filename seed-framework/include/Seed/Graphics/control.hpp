#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <AntTweakBar.h>

class Controller
{
public:
	Controller(GLFWwindow *window);
	~Controller();
	void updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV);
private:
	int context;
};
