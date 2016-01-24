//SEED INCLUDES
#include <Seed/Graphics/engine/tools.hpp>
#include <Seed/Graphics/engine/control.hpp>
#include <Seed/Graphics/engine/scene.hpp>
#include <Seed/Graphics/engine/camera.hpp>


int Controller::context = 0;

Controller::Controller(GLFWwindow *window)
{
	//captur keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(window, mouse_buttonID_callback);
	this->context = 0;
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
}

Controller::~Controller()
{
}


void Controller::updateControl(GLFWwindow* window, Camera *cam, float deltaTime)
{
	//position of the camera
	glm::vec3 position = cam->getPosition();
	//horizontal and vertical angle
	float WAngle = cam->getWAngle();
	float HAngle = cam->getHAngle();
	//speed move direction (keyboard)
	float speed = cam->getSpeed();
	//speed view direction (mouse)
	float mouseSpeed = cam->getMouseSpeed();
	glm::vec3 direction = cam->getDirection();
	glm::vec3 up = cam->getUp();
	glm::vec3 right = cam->getRight();

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
			direction = glm::vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));

			//get right direction of the camera
			right = glm::vec3(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
			//get the up direction of the camera
			up = glm::normalize(glm::cross(right, direction));
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
	}
}

void mouse_buttonID_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if action is press button
	if (action == GLFW_PRESS)
	{
		//we get the right and left button of the souris to send these to anttweakbar
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				Controller::context += 1;
				Controller::context %= 2;
				//Controller::context = 0;
				TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_RIGHT);
				break;
		}
	}
	//if user release button we do the same thing that above
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_LEFT:
				TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				//Controller::context = 1;
				TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_RIGHT);
				break;
		}
	}

}

void Controller::initAntWeakBar(std::string name)
{
	//initialisation AntWeakBar
	TwInit(TW_OPENGL_CORE, NULL);

	//windows size for anttweakbar
	TwWindowSize(WIDTH, HEIGHT);

	//initialize bar
	this->bar = TwNewBar(name.c_str());
}

TwBar* Controller::getBar()
{
	return this->bar;
}