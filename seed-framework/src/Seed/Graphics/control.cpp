#include <Seed/Graphics/control.hpp>
#include <Seed/Graphics/scene.hpp>
#include <Seed/Graphics/camera.hpp>

int Controller::context = 0;

Controller::Controller(GLFWwindow *window)
{
	//captur keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetMouseButtonCallback(window, mouse_buttonID_callback);
	this->context = 0;
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
	//field of view
	const float initFoV = cam->getInitFoV();
	float near = cam->getNear();
	float far = cam->getFar();
	//speed move direction (keyboard)
	float speed = cam->getSpeed();
	//speed view direction (mouse)
	float mouseSpeed = cam->getMouseSpeed();

	float FoV = cam->getInitFoV();
	glm::vec3 direction;
	glm::vec3 up;


	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Controller::context += 1;
		Controller::context %= 2;
	}

	double xpos, ypos;
	//get mouse position on the screen
	glfwGetCursorPos(window, &xpos, &ypos);

	if (Controller::context == 0)
	{
		//reset position of mouse
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
		//compute angles
		WAngle += mouseSpeed * float(WIDTH / 2 - xpos);
		HAngle += mouseSpeed * float(HEIGHT / 2 - ypos);

		//get direction of the camera
		direction = glm::vec3(cos(HAngle) * sin(WAngle), sin(HAngle), cos(HAngle) * cos(WAngle));
		//get right direction of the camera
		glm::vec3 right(sin(WAngle - 3.14f / 2.0f), 0, cos(WAngle - 3.14f / 2.0f));
		//get the up direction of the camera
		up = glm::normalize(glm::cross(right, direction));

		if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		{
			position += direction * speed;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS)
		{
			position -= direction * speed;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			position += right * speed;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			position -= right * speed;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			position += up * speed;
		}
		else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		{
			position -= up * speed;
		}
		//set the angle of the direction vector of the camera
		cam->setHAngle(HAngle);
		cam->setWAngle(WAngle);
		//update ViewMatrix
		cam->setViewMatrix(position, direction, up);
		//update Projection Matrix
		cam->setProjectionMatrix(FoV, WIDTH, HEIGHT, near, far);
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

void Controller::updateControl(GLFWwindow* window, float &WAngle, float &HAngle, float &mouseSpeed, float &deltaTime, float &speed, glm::vec3 &position, glm::vec3 &direction, glm::vec3 &up, const float &InitFoV, float &FoV)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Controller::context += 1;
		Controller::context %= 2;
	}

	double xpos, ypos;
	//get mouse position on the screen
	glfwGetCursorPos(window, &xpos, &ypos);

	if (Controller::context == 0)
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
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Scene::wireframe = !Scene::wireframe;
		}
	}
	else
	{
		//send position of the mouse to anttweakbar
		TwMouseMotion(xpos, ypos);
	}
}
