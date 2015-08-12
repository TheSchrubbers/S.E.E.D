#include "Control.hpp"

void updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV)
{
	double xpos, ypos;
	//get mouse podition on the screen
	glfwGetCursorPos(window, &xpos, &ypos);
	//reset position of mouse
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	//compute angles
	WAngle += mouseSpeed * deltaTime * float(WIDTH / 2 - xpos);
	HAngle += mouseSpeed * deltaTime * float(HEIGHT / 2 - ypos);

	direction = glm::vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));
	glm::vec3 right(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
	up = glm::cross(right, direction);

	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		position -= right * deltaTime * speed;
	}

	//FoV = InitFoV - 5 * glfwSetScrollCallback(window, )
}