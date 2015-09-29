
#include "Materials/DefaultMaterial2/DefaultMaterial2.hpp"
#include <Seed/Graphics/engine.hpp>

int main()
{
	//position of the camera
	glm::vec3 position = glm::vec3(0.0, 0.0, 5.0);

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

	engine.initAntWeakBar("Outils");

	//create scene
	Scene scene;
	//create camera
	Camera camera(position, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	unsigned int error;
	//adding camera to the scene
	scene.setCamera(&camera);

	scene.addLight(glm::vec3(0.0,0.0,5.0), glm::vec3(1.0), "light_1");

	//import model
	Node *cube1 = scene.importModelFromFile(pathToModels + "cube.obj", "cube1");

	if (cube1)
	{
		DefaultMaterial *material = new DefaultMaterial(&scene, "cube1_material", &error);
		scanSeedError(error);
		material->addTexture("texture1.bmp", &scene, TEXTURE_DIFFUSE, &error);
		scanSeedError(error);
		cube1->setMaterialRecur(material);
	}

	scene.addNode(cube1);
	
	Node *cube2 = scene.importModelFromFile(pathToModels + "cube.obj", "cube2");

	if (cube2)
	{
		DefaultMaterial *material = new DefaultMaterial(&scene, "cube2_material", &error);
		scanSeedError(error);
		material->addTexture("texture1.bmp", &scene, TEXTURE_DIFFUSE, &error);
		scanSeedError(error);
		cube2->setMaterialRecur(material);
		cube2->getMaterial()->translate(glm::vec3(0.0, 2.0, 0.0));
	}

	scene.addNode(cube2);

	scene.afficher();

	//collect all the rendered nodes
	scene.collectRenderedNodes();
	//active loop main render
	engine.mainRender(&scene);

	return 0;
}