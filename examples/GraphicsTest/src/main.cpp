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
#include <time.h>

// Open a window and create its OpenGL context 
GLFWwindow* window; // (In the accompanying source code, this variable is global) 

int Initialisation();

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
	//initialisation systeme
	if (Initialisation() != 0)
		return -1;

	//captur keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//create scene
	Scene scene;
	//import model
	scene.importModelFromFile(pathToModels + "cube.obj", "cube1");

	Node *node = scene.getNode("cube1");

	if (node)
	{
		std::cout << "trouve" << std::endl;
	}

	//create camera
	Camera camera(position, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0), initFoV, WIDTH, HEIGHT, near, far);

	scene.setCamera(&camera);
	
	//enable texturing
	//glEnable(GL_TEXTURE_2D);

	//GLuint TextureID = glGetUniformLocation(programID, "samplerTexture");
	
	//chargement texture
	//Texture t(pathToTextures + "texture2.bmp");

	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;
	float FoV = initFoV;
	glm::vec3 direction;
	glm::vec3 up;

	// Create and compile our GLSL program from the shaders
	GLuint programID = loadShaders(pathToShaders + "shader1\\VertexShader.hlsl", pathToShaders + "shader1\\FragmentShader.hlsl");//(pathToShaders + "\FragmentShader.hlsl").c_str());

	if (programID)
	{
		// Use our shader
		glUseProgram(programID);
	}


	glm::mat4 M = glm::mat4(1.0);
	//M = glm::translate(glm::mat4(1.0), glm::vec3(1.0, 0.0, 0.0));
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glm::mat4 MVP = scene.getCamera()->getProjectionMatrix() * camera.getViewMatrix() * M;

	//main loop to render
	do
	{
		//get current time
		currentTime = glfwGetTime();
		//update mouse control and keyboard control
		updateControl(window, WAngle, HAngle, mouseSpeed, deltaTime, speed, position ,  direction, up, initFoV, FoV);
		//update ViewMatrix
		scene.getCamera()->setViewMatrix(position, direction, up);
		//update Projection Matrix
		scene.getCamera()->setProjectionMatrix(FoV, WIDTH, HEIGHT, near, far);

		// Get a handle for our "MVP" uniform.
		// Only at initialisation time.
		GLuint MatrixID = glGetUniformLocation(programID, "MVP");

		// For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//update MVP matrix
		MVP = camera.getProjectionMatrix() * camera.getViewMatrix() * M;

		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		
		// Black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Accept fragment if it closer to the camera than the former one
		//glDepthFunc(GL_LESS);

		scene.getRootNode()->render();

		//on nettoie les buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		//get current time
		lastTime = glfwGetTime();
		//time between 2 frames
		deltaTime = float(lastTime - currentTime);


	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	
	// Cleanup VBO
	/*glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &ColorBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);*/
	
	
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
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
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	window = glfwCreateWindow(WIDTH, HEIGHT, "Moteur3d", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW 
	glewExperimental = true; // Needed in core profile 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	return 0;
}