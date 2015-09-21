#include <Seed/Graphics/control.hpp>

Controller::Controller(GLFWwindow *window)
{
	//captur keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	this->context = 0;
}

Controller::~Controller()
{
}

void Controller::updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		this->context += 1;
		this->context %= 2;
	}

	double xpos, ypos;
	//get mouse position on the screen
	glfwGetCursorPos(window, &xpos, &ypos);

	if (context == 0)
	{
		//reset position of mouse
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
		//compute angles
		WAngle += mouseSpeed * deltaTime * float(WIDTH / 2 - xpos);
		HAngle += mouseSpeed * deltaTime * float(HEIGHT / 2 - ypos);

			//get direction of the camera
		direction = glm::vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));
		//get right direction of the camera
		glm::vec3 right(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
		//get the up direction of the camera
		up = glm::cross(right, direction);

		if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		{
			position += direction * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
		{
			position -= direction * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			position += right * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			position -= right * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			position += up * deltaTime * speed;
		}
		if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		{
			position -= up * deltaTime * speed;
		}
	}
	else
	{
		//send position of the mouse to anttweakbar
		TwMouseMotion(xpos, ypos);
	}
}

