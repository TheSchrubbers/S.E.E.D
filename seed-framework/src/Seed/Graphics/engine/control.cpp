//SEED INCLUDES
#include "Seed/Graphics/engine/tools.hpp"
#include "Seed/Graphics/engine/control.hpp"
#include "Seed/Graphics/engine/scene.hpp"
#include "Seed/Graphics/engine/camera.hpp"

using namespace glm;
using namespace std;

Controller::Controller()
{
}

Controller::~Controller()
{
}


void Controller::updateControl(Camera *cam, float deltaTime)
{
	//position of the camera
	/*vec3 position = cam->getPosition();
	//horizontal and vertical angle
	float WAngle = cam->getWAngle();
	float HAngle = cam->getHAngle();
	//speed move direction (keyboard)
	float speed = cam->getSpeed();
	//speed view direction (mouse)
	float mouseSpeed = cam->getMouseSpeed();
	vec3 direction = cam->getDirection();
	vec3 up = cam->getUp();
	vec3 right = cam->getRight();

	bool dragMouse = false, keyInput = false;

	double xpos, ypos;
	//get mouse position on the screen
	glfwGetCursorPos(window, &xpos, &ypos);


	if (Controller::context == 0)
	{
		if(xpos != WIDTH / 2.0f || ypos != HEIGHT / 2.0f)
		{
			dragMouse = true;
			//reset position of mouse
			glfwSetCursorPos(window, WIDTH / 2.0f, HEIGHT / 2.0f);

			//compute angles
			WAngle += mouseSpeed * ((WIDTH / 2.0f) - xpos);
			HAngle += mouseSpeed * ((HEIGHT / 2.0f) - ypos);

			//get direction of the camera
			direction = vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));

			//get right direction of the camera
			right = vec3(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
			//get the up direction of the camera
			up = normalize(cross(right, direction));
		}

		//get inputs key
		if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		{
			position += direction * speed;
			keyInput = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
		{
			position -= direction * speed;
			keyInput = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			position += right * speed;
			keyInput = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			position -= right * speed;
			keyInput = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			position += up * speed;
			keyInput = true;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		{
			position -= up * speed;
			keyInput = true;
		}

		if(dragMouse)
		{
			//set the angle of the direction vector of the camera
			cam->setHAngle(HAngle);
			cam->setWAngle(WAngle);
			cam->setRight(right);
			//update ViewMatrix
			cam->setViewMatrix(position, direction, up);
			
		}
		else if(keyInput)
		{
			cam->setViewMatrix(position);
		}
	}
	else
	{
		//send position of the mouse to anttweakbar
		TwMouseMotion(xpos, ypos);
		if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_0, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_1, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_2, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_3, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_4, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_5, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_6, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_7, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_8, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_9, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_KP_ENTER, GLFW_PRESS);
		}
		else if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)
		{
			TwEventKeyGLFW(GLFW_KEY_DELETE, GLFW_PRESS);
		}
	}*/
}

void Controller::updateControlMoveMouse(Camera *cam, float WAngle, float HAngle, double deltaX, double deltaY)
{
	//position of the camera
	vec3 position = cam->getPosition();
	//speed view direction (mouse)
	float mouseSpeed = cam->getMouseSpeed();

	//compute angles
	WAngle += mouseSpeed * (deltaX);
	HAngle += mouseSpeed * (deltaY);

	//get direction of the camera
	vec3 direction = vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));

	//get right direction of the camera
	vec3 right = vec3(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
	//get the up direction of the camera
	vec3 up = normalize(cross(right, direction));

	//set the angle of the direction vector of the camera
	cam->setHAngle(HAngle);
	cam->setWAngle(WAngle);
	cam->setRight(right);
	//update ViewMatrix
	cam->setViewMatrix(position, direction, up);
}

void Controller::updateDragMouseControl(Camera *cam, float deltaPosX, float deltaPosY)
{
	if(deltaPosX > 40)
		deltaPosX = 40.0f;
	if(deltaPosY > 40)
		deltaPosY = 40.0f;
	//position of the camera
	vec3 position = cam->getPosition();
	//horizontal and vertical angle
	float WAngle = cam->getWAngle();
	float HAngle = cam->getHAngle();
	//speed view direction (mouse)
	float mouseSpeed = cam->getMouseSpeed();

	//compute angles
	WAngle += mouseSpeed * (-deltaPosX);
	HAngle += mouseSpeed * (-deltaPosY);

	//get direction of the camera
	vec3 direction = glm::vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));
	//get right direction of the camera
	vec3 right = glm::vec3(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
	//get the up direction of the camera
	vec3 up = glm::normalize(glm::cross(right, direction));

	//set the angle of the direction vector of the camera
	cam->setHAngle(HAngle);
	cam->setWAngle(WAngle);
	cam->setRight(right);
	//update ViewMatrix
	cam->setViewMatrix(position, direction, up);
}