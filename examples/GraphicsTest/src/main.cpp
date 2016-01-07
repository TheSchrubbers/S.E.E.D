#include <Seed/Graphics/engine.hpp>

void addLara(Scene *scene);
void addHouse(Scene *scene);

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
	Scene scene;
	//create camera
	Camera camera(position, glm::vec3(0.0, 0.0, 0.0));

	engine.initAntWeakBar("TOOLS", &camera);

	//adding camera to the scene
	scene.setCamera(&camera);

	//scene.setCubeMap(pathToTextures + "CubeMap/Skybox");

	scene.addPointLight(glm::vec3(3.0,8.0,-5.0), glm::vec3(1.0), glm::vec3(0.1f, 0.8f, 0.8f), 50, "light_1");
	scene.addDirectionnalLight(glm::vec3(1.0), glm::normalize(glm::vec3(0.0) - glm::vec3(0.0, 0.0, 5.0)), glm::vec3(0.1f, 0.8f, 0.8f));
	//scene.addSpotLight(glm::vec3(0.0, 3.0, 5.0), glm::normalize(glm::vec3(0.0) - glm::vec3(0.0, 3.0, 5.0)), glm::vec3(1.0), 10);
	
	//scene.addWaterSystemParticles(glm::vec3(0.0), SEED_POINT, 50, "WaterSystemParticles");


	//addLara(&scene); 
	//addHouse(&scene);

	ObjectNode *cube = scene.importModelFromFile(pathToModels + "cube.obj", "cube");
	DeferredMaterial *material = new DeferredMaterial(&scene, "cube_material", nullptr);
	cube->setMaterialRecur(material);

	scene.addNode(cube);

	/*SPHMaterial *m = new SPHMaterial(&scene, "SPHMaterial", nullptr);
	ObjectNode *n = scene.importModelFromFile(pathToModels + "UVsphereLow.obj", "UVSphere");
	n->setMaterialRecur(m);

	scene.addNode(n);*/

	//ObjectNode *n = new ObjectNode(&scene);
	//ImplicitMaterial *mat = new ImplicitMaterial(&scene, "explicit-material");
	//n->setMaterial(mat);
	//scene.addNode(n);

	scene.afficher();

	//collect all the rendered nodes
	scene.collectRenderedNodes();
	//active loop main render
	engine.mainRender(&scene);

	return 0;
}

void addLara(Scene *scene)
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

void addHouse(Scene *scene)
{
	unsigned int error;
	//import model
	ObjectNode *house = scene->importModelFromFile(pathToModels + "GostHouse.obj", "ghost_house");

	ObjectNode *n = house->getNode("Gost_House");
	if (n)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "GostHouse_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->scaleModel(glm::vec3(0.1f));
		material->addTexture("House/House_Body.bmp", scene, SEED_TEXTURE_AMBIANT);
		n->setMaterialRecur(material);
	}
	n = house->getNode("plants");
	if (n)
	{
		DefaultMaterial *material = new DefaultMaterial(scene, "Plants_material", &error, 0.0, 0.0);
		scanSeedError(error);
		material->scaleModel(glm::vec3(0.1f));
		material->addTexture("House/grass.bmp", scene, SEED_TEXTURE_AMBIANT);
		n->setMaterialRecur(material);
	}

	scene->addNode(house);
}

//import model
/*ObjectNode *cube1 = scene.importModelFromFile(pathToModels + "cube.obj", "cube1");

if (cube1)
{
DefaultMaterial *material = new DefaultMaterial(&scene, "cube1_material", 0.0, 0.0, &error);
scanSeedError(error);
material->addTexture("bois.png", &scene, TEXTURE_AMBIANT, &error);
scanSeedError(error);
cube1->setMaterialRecur(material);
}

scene.addNode(cube1);*/

/*ObjectNode *cube2 = scene.importModelFromFile(pathToModels + "cube.obj", "cube2");

if (cube2)
{
DefaultMaterial *material = new DefaultMaterial(&scene, "cube2_material", &error);
scanSeedError(error);
material->addTexture("texture1.png", &scene, TEXTURE_AMBIANT, &error);
scanSeedError(error);
cube2->setMaterialRecur(material);
cube2->getMaterial()->translate(glm::vec3(0.0, 2.0, 0.0));
}

scene.addNode(cube2);*/