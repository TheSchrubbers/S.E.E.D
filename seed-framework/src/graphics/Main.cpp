#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h
#ifdef _WIN32
	#include <GL\glew.h>
#else
	#include <GL/glew.h>
#endif

// Include GLFW
#ifdef _WIN32
	#include <GLFW\glfw3.h>
#else
	#include <GLFW/glfw3.h>
#endif

// Include GLM
#ifdef _WIN32
	#include <glm\glm.hpp>
	#include <glm\gtx\transform.hpp>
#else
	#include <glm/glm.hpp>
	#include <glm/gtx/transform.hpp>
#endif

#include "Camera.hpp"
#include "Outils.hpp"

#include <time.h>
#include "Texture.hpp"
#include "Model.hpp"
#include "Control.hpp"
#include "Constant.hpp"
// Include standard headers
#include "Shader.hpp"
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
	float near = 0.1;
	float far = 100.0;
	//speed move direction (keyboard)
	float speed = 3.0f;
	//speed view direction (mouse)
	float mouseSpeed = 0.05f;
	//initialisation systeme
	if (Initialisation() != 0)
		return -1;

	//captur keys
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//crate camera
	Camera camera(position, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0), initFoV, WIDTH, HEIGHT, near, far);

	// Create and compile our GLSL program from the shaders
	GLuint programID = 0;// loadShaders("VertexShader.hlsl", "PixelShader.hlsl");//(pathToShaders + "\FragmentShader.hlsl").c_str());
	GLuint program = loadShaders("tes2", "tes3");

	//enable texturing
	glEnable(GL_TEXTURE_2D);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	/*static const GLfloat vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};*/

	// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
	static const GLfloat uv_buffer_data[] = {
		0.000059f, 1.0f - 0.000004f,
		0.000103f, 1.0f - 0.336048f,
		0.335973f, 1.0f - 0.335903f,
		1.000023f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.336024f, 1.0f - 0.671877f,
		0.667969f, 1.0f - 0.671889f,
		1.000023f, 1.0f - 0.000013f,
		0.668104f, 1.0f - 0.000013f,
		0.667979f, 1.0f - 0.335851f,
		0.000059f, 1.0f - 0.000004f,
		0.335973f, 1.0f - 0.335903f,
		0.336098f, 1.0f - 0.000071f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.336024f, 1.0f - 0.671877f,
		1.000004f, 1.0f - 0.671847f,
		0.999958f, 1.0f - 0.336064f,
		0.667979f, 1.0f - 0.335851f,
		0.668104f, 1.0f - 0.000013f,
		0.335973f, 1.0f - 0.335903f,
		0.667979f, 1.0f - 0.335851f,
		0.335973f, 1.0f - 0.335903f,
		0.668104f, 1.0f - 0.000013f,
		0.336098f, 1.0f - 0.000071f,
		0.000103f, 1.0f - 0.336048f,
		0.000004f, 1.0f - 0.671870f,
		0.336024f, 1.0f - 0.671877f,
		0.000103f, 1.0f - 0.336048f,
		0.336024f, 1.0f - 0.671877f,
		0.335973f, 1.0f - 0.335903f,
		0.667969f, 1.0f - 0.671889f,
		1.000004f, 1.0f - 0.671847f,
		0.667979f, 1.0f - 0.335851f
		};

	/*static const GLfloat uv_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};*/

	srand(time(NULL));
	static GLfloat color_buffer_data[12 * 3 * 3];
	for (int i = 0; i < 12*3; i++)
	{
		color_buffer_data[i * 3] = (float)(rand() % 100) / 100.0;
		color_buffer_data[i * 3 + 1] = (float)(rand() % 100) / 100.0;
		color_buffer_data[i * 3 + 2] = (float)(rand() % 100) / 100.0;
	}

	//id vertex VertexArrayID
	GLuint VertexBuffer;
	//on génère une adresse pointant vers un emplacement dans la mémoire du GPU
	//on génère 1 emplacement
	glGenBuffers(1, &VertexBuffer);
	//on bind l'emplacement
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	//on donne les vertex à openGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	//id du color vertexArrayID
	GLuint ColorBuffer;
	glGenBuffers(1, &ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

	//id UV vertexArrayID
	GLuint UVBuffer;
	glGenBuffers(1, &UVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);

	
	glm::mat4 M = glm::mat4(1.0);
	//M = glm::translate(glm::mat4(1.0), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 MVP = camera.getProjectionMatrix() * camera.getViewMatrix() * M;

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint TextureID = glGetUniformLocation(programID, "samplerTexture");
	
	//chargement texture
	Texture t("\texture4.png");


	//Model model("Models/axe.obj");

	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;
	float FoV = initFoV;
	glm::vec3 direction;
	glm::vec3 up;

	//main loop to render
	do
	{
		//get current time
		currentTime = glfwGetTime();
		//update mouse control and keyboard control
		updateControl(window, WAngle, HAngle, mouseSpeed, deltaTime, speed, position ,  direction, up, initFoV, FoV);
		//printf("%f %f %f\n", FoV, near, far);
		//update ViewMatrix
		camera.setViewMatrix(position, direction, up);
		//update Projection Matrix
		camera.setProjectionMatrix(FoV, WIDTH, HEIGHT, near, far);
		//update MVP matrix
		MVP = camera.getProjectionMatrix() * camera.getViewMatrix() * M;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//vertexs
		//on se place sur les vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(
			0,//canal 0
			3,//la taille du vertex
			GL_FLOAT,//le type de données
			GL_FALSE,//est-ce que les données sont normalisée
			0,//stride
			(void*)0//offset du buffer
			);


		//couleurs
		//on se place sur les couleurs
		glEnableVertexAttribArray(1);
		//on bind le buffer
		glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);

		//on attribue le buffer sur le canal 1
		glVertexAttribPointer(
			1,//canal 1
			3,//la taille de la couleur
			GL_FLOAT,//le type de données
			GL_FALSE,//est-ce que les données sont normalisée
			0,//stride
			(void*)0//offset du buffer
		);

		//UV
		//on se place sur le canal pour les UV
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
		glVertexAttribPointer(
			2,//canal 0
			2,//la taille du vertex
			GL_FLOAT,//le type de données
			GL_FALSE,//est-ce que les données sont normalisée
			0,//stride
			(void*)0//offset du buffer
			);

		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		t.bind();
		glUniform1i(TextureID, 0);

		//draw Arrays!
		glDrawArrays(GL_TRIANGLES, 0, 12*3);//on part du vertex0 et il y en a trois
		t.release();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//on nettoie les buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		//get current time
		lastTime = glfwGetTime();
		//time between 2 frames
		deltaTime = float(lastTime - currentTime);


	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &ColorBuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

int Initialisation()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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