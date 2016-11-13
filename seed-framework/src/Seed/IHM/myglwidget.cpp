#include "Seed/Graphics/engine/engine.hpp"
#include "Seed/IHM/myglwidget.hpp"
#include <QMessageBox>
#include <QTimer>
#include <QObject>
#include <QStatusBar>
#include <QTimer>

using namespace std;
using glm::vec3;

void addLara(std::shared_ptr<Scene> scene);

MyGLWidget::MyGLWidget(QStatusBar *qSB, QWidget *parent) 
    : GLWidget(60, parent)
{
	m_engine = new Engine();
    setFocusPolicy(Qt::StrongFocus);
    timerFPS = new QTimer(this);
    connect(timerFPS, SIGNAL(timeout()), this, SLOT(updateFPSPrint()));
    timerFPS->start(1000);
    qStatusBar = qSB;
    FPS = 0;
}

MyGLWidget::~MyGLWidget() 
{
}

void MyGLWidget::initializeGL()
{
    cout << "initialize GL" << endl;
    makeCurrent();
    initializeOpenGLFunctions();
    m_engine->initController();
	shared_ptr<Scene> scene = m_engine->createScene();
	//create camera
	Camera *camera = new Camera(glm::vec3(0.0, 0.0, 8.0), glm::vec3(0.0, 0.0, 0.0));
    camera->setMouseSpeed(1.0f/500.0f);
	//adding camera to the scene
	scene->setCamera(camera);

	//scene->addPointLight(glm::vec3(0.0,3.0,8.0), glm::vec3(1.0), glm::vec3(0.1f, 0.8f, 0.8f), 1.0f, 30.0f, 0.0f, "light_1");

    //scene->setCubeMap(pathToTextures + "CubeMap/Skybox");

    //cout << "setCubeMap fine" << endl;

	unsigned int error;
	/*ObjectNode *brique = scene->importModelFromFile(pathToBasicModels + "plan2.obj", "briques");
	brique->setShadowMapped(false);
	DefaultMaterial *parallax = new DefaultMaterial(scene, "parallax_material", &error, 0.02, 0.0);
	//ColorMaterial *parallax = new ColorMaterial(scene, "parallax_material", glm::vec3(0.5, 0.5, 0.5), &error);
	parallax->addTexture("parallax/diffuse.png", scene, SEED_TEXTURE_AMBIANT);
	parallax->addTexture("parallax/normal.png", scene, SEED_TEXTURE_NORMAL);
	parallax->addTexture("parallax/parallax.png", scene, SEED_TEXTURE_DEPTHMAP);
	parallax->scaleModel(glm::vec3(10.0));
	parallax->translateModel(glm::vec3(5.0, -1.0, 0.0));
	parallax->rotateModel(glm::vec3(0.0,0.0,0.5));
	brique->setMaterialRecur(parallax);
	scene->addNode(brique);*/

    ObjectNode *monkey = scene->importModelFromFile(pathToModels + "BasicMesh/cube.obj", "monkey");
    SimpleMaterial *material = new SimpleMaterial(scene, "material", &error);
    monkey->setMaterialRecur(material);
    scene->addNode(monkey);

    cout << "addLara" << endl;
   
    //addLara(scene);

    cout << "fin add Lara" << endl;

    cout << "collectRendedNodes" << endl;

	//collect all the rendered nodes
	scene->collectRenderedNodes();

    cout << "Fin collectRendedNodes" << endl;
    cout << "loadSystemRendering" << endl;


    m_engine->loadSystemToRendering();
    cout << "Fin loadSystemRendering" << endl;

    //glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MyGLWidget::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
}

void MyGLWidget::paintGL()
{
    makeCurrent();
	m_engine->mainRender();
    FPS++;
}

void MyGLWidget::updateFPSPrint()
{
    qStatusBar->showMessage("FPS: " + QString::number(FPS));
    FPS = 0;
    timerFPS->start(1000);
}

void addLara(std::shared_ptr<Scene> scene)
{
    unsigned int error;
    //import model
    ObjectNode *lara = scene->importModelFromFile(pathToModels + "Lara_Croft.obj", "lara");

    ObjectNode *l = lara->getNode("LaraClothes");
    if (l)
    {
        cout << "defaultMaterial contructor" << endl;
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraClothes_material", &error, 0.0, 0.0);
        cout << "scaneError" << endl;
        scanSeedError(error);
        cout << "add texture 1" << endl;
        material->addTexture("Lara_croft/Lara_Clothes_D.tga", scene, SEED_TEXTURE_AMBIANT);
        cout << "add texture 2" << endl;
        material->addTexture("Lara_croft/Lara_Clothes_S.tga", scene, SEED_TEXTURE_SPECULAR);
        cout << "add texture 3" << endl;
        material->addTexture("Lara_croft/Lara_Clothes_N.tga", scene, SEED_TEXTURE_NORMAL);
        cout << "set material recur" << endl;*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraClothes_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraSkin");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraSkin_material", &error, 0.02, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Skin_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Skin_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Skin_N.tga", scene, SEED_TEXTURE_NORMAL);*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraSkin_material", vec3(1.0), &error);

        SimpleMaterial *material = new SimpleMaterial(scene, "skinMaterial", &error);
        l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraHead");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraHead_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Head_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Head_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Head_N.tga", scene, SEED_TEXTURE_NORMAL);*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraHead_material", vec3(1.0), &error);
       // l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraEquipment");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraEquipment_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Equipment_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Equipment_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Equipment_N.tga", scene, SEED_TEXTURE_NORMAL);*/
       // ColorMaterial *material = new ColorMaterial(scene, "laraEquipment_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraEye");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraEye_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Eye_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Eye_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Eye_N.tga", scene, SEED_TEXTURE_NORMAL);*/
      //  ColorMaterial *material = new ColorMaterial(scene, "laraEye_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("PistolLegs");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraPistol_material", &error, 0.2, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Pistol_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Pistol_N.tga", scene, SEED_TEXTURE_NORMAL);*/
       // ColorMaterial *material = new ColorMaterial(scene, "laraPistol_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("PistolHands");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraPistol_material", &error, 0.2, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Pistol_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Pistol_N.tga", scene, SEED_TEXTURE_NORMAL);*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraPistol_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraHairTie");
    if (l)
    {
        /**DefaultMaterial *material = new DefaultMaterial(scene, "laraHairTie_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Hair_Tie_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Hair_Tie_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Hair_Tie_N.tga", scene, SEED_TEXTURE_NORMAL);*/
       // ColorMaterial *material = new ColorMaterial(scene, "laraHairTie_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraHair_1_Mesh_2_0");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraHair_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Hair_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Hair_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Hair_N.tga", scene, SEED_TEXTURE_NORMAL);*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraHair_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
    }

    l = lara->getNode("LaraHair_2");
    if (l)
    {
        /*DefaultMaterial *material = new DefaultMaterial(scene, "laraHair2_material", &error, 0.0, 0.0);
        scanSeedError(error);
        material->addTexture("Lara_croft/Lara_Hair_D.tga", scene, SEED_TEXTURE_AMBIANT);
        material->addTexture("Lara_croft/Lara_Hair_S.tga", scene, SEED_TEXTURE_SPECULAR);
        material->addTexture("Lara_croft/Lara_Hair_N.tga", scene, SEED_TEXTURE_NORMAL);*/
        //ColorMaterial *material = new ColorMaterial(scene, "laraHair2_material", vec3(1.0), &error);
        //l->setMaterialRecur(material);
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

    cout << "scene add node" << endl;
    scene->addNode(lara);
}



