﻿#include <Seed/Graphics/engine/engine.hpp>

void addLara(std::shared_ptr<Scene> scene);

int main()
{
	//position of the camera
	glm::vec3 position = glm::vec3(0.0, 0.0, 8.0);

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

	//create scene
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	//create camera
	Camera camera(position, glm::vec3(0.0, 0.0, 0.0));

	engine.initAntWeakBar("TOOLS", &camera);

	//adding camera to the scene
	scene->setCamera(&camera);

	//scene.setCubeMap(pathToTextures + "CubeMap/Skybox");

	scene->addPointLight(glm::vec3(3.0,8.0,-5.0), glm::vec3(1.0), glm::vec3(0.1f, 0.8f, 0.8f), 50, "light_1");
	scene->addDirectionnalLight(glm::vec3(1.0), glm::normalize(glm::vec3(0.0) - glm::vec3(0.0, 0.0, 5.0)), glm::vec3(0.1f, 0.8f, 0.8f));
	//scene.addSpotLight(glm::vec3(0.0, 3.0, 5.0), glm::normalize(glm::vec3(0.0) - glm::vec3(0.0, 3.0, 5.0)), glm::vec3(1.0), 10);
	
	//scene.addWaterSystemParticles(glm::vec3(0.0), SEED_POINT, 50, "WaterSystemParticles");


	addLara(scene); 
	/*unsigned int error;
	ObjectNode *cube = scene.importModelFromFile(pathToBasicModels + "Monkey.obj", "cube");
	DefaultMaterial *material = new DefaultMaterial(&scene, "cube_material", &error);
	scanSeedError(error);
	//material->translateModel(glm::vec3(2.0, 0.0, 1.0));
	cube->setMaterialRecur(material);
	scene.addNode(cube);*/

	/*ObjectNode *cube2 = scene.importModelFromFile(pathToBasicModels + "cube.obj", "cube");
	DefaultMaterial *material2 = new DefaultMaterial(&scene, "cube2_material", nullptr);
	material->rotateModel(glm::vec3(0.0, 0.2, 0.0));
	cube2->setMaterialRecur(material2);
	scene.addNode(cube2);

	ObjectNode *sphere = scene.importModelFromFile(pathToBasicModels + "sphere.obj", "cube");
	DefaultMaterial *material3 = new DefaultMaterial(&scene, "sphere_material", nullptr);
	material3->translateModel(glm::vec3(-1.0, 2.0, 2.0));
	sphere->setMaterialRecur(material3);
	scene.addNode(sphere);

	ObjectNode *plan = scene.importModelFromFile(pathToBasicModels + "plan.obj", "cube");
	DefaultMaterial *material4 = new DefaultMaterial(&scene, "plan_material", nullptr);
	material4->scaleModel(glm::vec3(3.0, 3.0, 3.0));
	material4->translateModel(glm::vec3(0.0, -0.5, 0.0));
	plan->setMaterialRecur(material4);
	scene.addNode(plan);*/

	/*SPHMaterial *m = new SPHMaterial(&scene, "SPHMaterial", nullptr);
	ObjectNode *n = scene.importModelFromFile(pathToBasicModels + "UVsphereLow.obj", "UVSphere");
	n->setMaterialRecur(m);

	scene.addNode(n);*/

	//ObjectNode *n = new ObjectNode(&scene);
	//ImplicitMaterial *mat = new ImplicitMaterial(&scene, "explicit-material");
	//n->setMaterial(mat);
	//scene.addNode(n);

	scene->afficher();

	//collect all the rendered nodes
	scene->collectRenderedNodes();
	//active loop main render
	engine.mainRender(scene);
	return 0;
}

void addLara(std::shared_ptr<Scene> scene)
{
	unsigned int error;
	//import model
	ObjectNode *lara = scene->importModelFromFile(pathToModels + "Lara_Croft.obj", "lara");

	ObjectNode *l = lara->getNode("LaraClothes");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraClothes_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Clothes_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Clothes_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Clothes_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraSkin");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraSkin_material", &error, 0.02, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Skin_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Skin_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Skin_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraHead");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraHead_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Head_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Head_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Head_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraEquipment");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraEquipment_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Equipment_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Equipment_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Equipment_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraEye");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraEye_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Eye_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Eye_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Eye_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("PistolLegs");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraPistol_material", &error, 0.2, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Pistol_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Pistol_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("PistolHands");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraPistol_material", &error, 0.2, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Pistol_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Pistol_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraHairTie");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraHairTie_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Hair_Tie_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Hair_Tie_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Hair_Tie_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraHair_1_Mesh_2_0");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraHair_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Hair_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Hair_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Hair_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	l = lara->getNode("LaraHair_2");
	if (l)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "laraHair2_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->addTexture("Lara_croft/Lara_Hair_D.tga", scene, SEED_TEXTURE_AMBIANT);
		material->addTexture("Lara_croft/Lara_Hair_S.tga", scene, SEED_TEXTURE_SPECULAR);
		material->addTexture("Lara_croft/Lara_Hair_N.tga", scene, SEED_TEXTURE_NORMAL);
		l->setMaterialRecur(material);
	}

	/*l = lara->getNode("LaraEyebrowEyelash");
	if (l)
	{
	DefaultMaterial *material = new DefaultMaterial(&scene, "laraBrownEyeLash_material", &error);
	scanSeedError(error);
	material->addTexture("Lara_croft/Lara_Eyebrows_Eyelashes_D.tga", &scene, TEXTURE_AMBIANT);
	material->addTexture("Lara_croft/Lara_Eyebrows_Eyelashes_S.tga", &scene, TEXTURE_SPECULAR);
	l->setMaterialRecur(material);
	}*/

	scene->addNode(lara);

}