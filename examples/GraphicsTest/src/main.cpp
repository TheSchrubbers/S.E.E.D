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
#include <Seed/Graphics/engine.hpp>

#define GLFW_INCLUDE_GLCOREARB

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

	float FoV = initFoV;

	//INIT ENGINE 
	Engine engine;

	if (!engine.initSystem())
	{
		return -1;
	}

	if (!engine.initController())
	{
		return -1;
	}

	//initialisation AntWeakBar
	TwInit(TW_OPENGL_CORE, NULL);

	//windows size for anttweakbar
	TwWindowSize(WIDTH, HEIGHT);

	TwBar *myBar;
	myBar = TwNewBar("Outils");

	glfwSetMouseButtonCallback(engine.window, mouse_buttonID_callback);

	//create scene
	Scene scene;
	//create camera
	Camera camera(position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), initFoV, WIDTH, HEIGHT, near, far, WAngle, HAngle, speed, mouseSpeed);
	//adding camera to the scene
	scene.setCamera(&camera);

	unsigned int error;

	scene.addLight(glm::vec3(0.0,0.0,5.0), glm::vec3(1.0), "light_1");

	//import model
	scene.importModelFromFile(pathToModels + "cube.obj", "cube1");

	Node *node = scene.getNode("cube1");
	if (node)
	{
		DefaultMaterial *material = new DefaultMaterial(&scene, "node_material", &error);
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

	scene.collectRenderedNodes();

	engine.mainRender(&scene);

	return 0;
}