#include <Seed/Graphics/engine.hpp>

Engine::Engine()
{
}

Engine::~Engine()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	TwTerminate();
}

void Engine::mainRender(Scene *scene)
{
	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;

	//main loop to render
	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


		//get events glfw
		glfwPollEvents();
		//get current time
		currentTime = glfwGetTime();

		//update mouse control and keyboard control
		//this->controller->updateControl(this->window, WAngle, HAngle, mouseSpeed, deltaTime, speed, position, direction, up, initFoV, FoV);
		this->controller->updateControl(this->window, scene->getCamera(), deltaTime);

		//node->getMaterial()->setLight(a, d, s);
		scene->render();

		//Draw anttweakbar
		TwDraw();

		//get current time
		lastTime = glfwGetTime();
		//time between 2 frames
		deltaTime = float(lastTime - currentTime);

		//on nettoie les buffers
		glfwSwapBuffers(this->window);

	} while (glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(this->window) == 0);
}
bool Engine::initSystem()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	window = glfwCreateWindow(WIDTH, HEIGHT, "Moteur3d", NULL, NULL);

	if (window == NULL){
		std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW 
	glewExperimental = GL_TRUE; // Needed in core profile 

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW, version of opengl must be greater or equal than opengl 3.2\n" << std::endl;
		return false;
	}

	std::cout << window << std::endl;
	return true;
}

bool Engine::initController()
{
	this->controller = new Controller(window);
	return true;
}