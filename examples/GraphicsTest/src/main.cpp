#include <stdio.h>
#include <stdlib.h>
//*
// Include GLEW. Always include it before gl.h and glfw.h

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <Seed/Graphics/camera.hpp>
#include <Seed/Graphics/texture.hpp>
#include <Seed/Graphics/model.hpp>
#include <Seed/Graphics/shader.hpp>
#include <Seed/Graphics/control.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <Seed/Graphics/Outils.hpp>
#include <Seed/Graphics/scene.hpp>
#include <DefaultMaterial/DefaultMaterial.hpp>
#include "Materials/DefaultMaterial2/DefaultMaterial2.hpp"
#include <time.h>
#include <AntTweakBar.h>

#define GLFW_INCLUDE_GLCOREARB

// Open a window and create its OpenGL context 
GLFWwindow* window; // (In the accompanying source code, this variable is global) 

int Initialisation();
void mouse_buttonID_callback(GLFWwindow* window, int button, int action, int mods);

int main()
{

	//position of the camera
	glm::vec3 position = glm::vec3(0.0, 0.0, 5.0);
	//horizontal and vertical angle
	float WAngle = 3.14f;
	float HAngle = 0.0f;
	//field of view
	const float initFoV = 45.0f;
	float near = (float)0.1;
	float far = (float)100.0;
	//speed move direction (keyboard)
	float speed = 3.0f;
	//speed view direction (mouse)
	float mouseSpeed = 0.05f;


	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;
	float FoV = initFoV;
	glm::vec3 direction;
	glm::vec3 up;

	//initialisation systeme
	if (Initialisation() != 0)
		return -1;

	Controller controller(window);

	//initialisation AntWeakBar
	TwInit(TW_OPENGL_CORE, NULL);

	//windows size for anttweakbar
	TwWindowSize(WIDTH, HEIGHT);

	TwBar *myBar;
	myBar = TwNewBar("Outils");

	glfwSetMouseButtonCallback(window, mouse_buttonID_callback);

	//create scene
	Scene scene;
	//create camera
	Camera camera(position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), initFoV, WIDTH, HEIGHT, near, far);
	//adding camera to the scene
	scene.setCamera(&camera);

	unsigned int error;

	scene.addLight(glm::vec3(0.0), glm::vec3(1.0), "light_1");

	//import model
	scene.importModelFromFile(pathToModels + "cube.obj", "cube1");

	Node *node = scene.getNode("cube1");
	if (node)
	{
		DefaultMaterial *material = new DefaultMaterial(scene.getCamera(), "node_material", &error);
		scanSeedError(error);
		material->addTexture("texture1.bmp", &scene, TEXTURE_DIFFUSE, &error);
		scanSeedError(error);
		node->setMaterialRecur(material);
	}

	float a = 0.1f, d = 0.5f, s = 0.4f;
	TwAddVarRW(myBar, "Ambiant composant", TW_TYPE_FLOAT, &a, " min=0.0 max=1.0 step=0.01 group=Engine label='Ambiant composant' ");
	TwAddVarRW(myBar, "Diffuse composant", TW_TYPE_FLOAT, &d, " min=0.0 max=1.0 step=0.01 group=Engine label='Diffuse composant' ");
	TwAddVarRW(myBar, "Specular composant", TW_TYPE_FLOAT, &s, " min=0.0 max=1.0 step=0.01 group=Engine label='Specular composant' ");
	/*scene.importModelFromFile(pathToModels + "cube.obj", "light");

	Node *lightNode = scene.getNode("light");
	if (lightNode)
	{
		DefaultMaterial2 *material2 = new DefaultMaterial2(scene.getCamera(), "light_material", &error);
		scanSeedError(error);
		lightNode->setMaterialRecur(material2);
	}*/
	
	/*scene.importModelFromFile(pathToModels + "cube.obj", "cube2");

	Node *node2 = scene.getNode("cube2");
	if (node2)
	{
		DefaultMaterial2 *material2 = new DefaultMaterial2(scene.getCamera(), "node2_material", &error);
		scanSeedError(error);
		material2->addTexture("texture1.bmp", &scene, TEXTURE_DIFFUSE, &error);
		scanSeedError(error);
		node2->setMaterialRecur(material2);
	}*/

	//enable texturing
	//glEnable(GL_TEXTURE_2D);

	//scene.lightsRendering();
	scene.collectRenderedNodes();

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
		controller.updateControl(window, WAngle, HAngle, mouseSpeed, deltaTime, speed, position,  direction, up, initFoV, FoV);
		//update ViewMatrix
		scene.getCamera()->setViewMatrix(position, direction, up);
		//update Projection Matrix
		scene.getCamera()->setProjectionMatrix(FoV, WIDTH, HEIGHT, near, far);


		// Accept fragment if it closer to the camera than the former one
		//glDepthFunc(GL_LESS);

		node->getMaterial()->setLight(a, d, s);

		//scene.getRootNode()->render();
		scene.render();

		//Draw anttweakbar
		TwDraw();

		//get current time
		lastTime = glfwGetTime();
		//time between 2 frames
		deltaTime = float(lastTime - currentTime);

		//on nettoie les buffers
		glfwSwapBuffers(window);

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);		
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	TwTerminate();
	return 0;
}

int Initialisation()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "Moteur3d", NULL, NULL);

	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW 
	glewExperimental = GL_TRUE; // Needed in core profile 

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW, version of opengl must be greater or equal than opengl 3.2\n");
		return -1;
	}
	return 0;
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
