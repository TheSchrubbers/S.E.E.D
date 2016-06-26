#include <Seed/Graphics/engine/engine.hpp>
#include <glwidget.hpp>
#include <QMessageBox>

GlWidget::GlWidget(int framePerSecond, QWidget *parent):QGLWidget(parent)
{
	if(framePerSecond == 0)
		t_timer = nullptr;
	else
	{
		int seconde = 1000;
		int timerInterval = seconde / framePerSecond;
		t_timer = new QTimer(this);
		connect(t_timer, SIGNAL(timeout()))
	}
}

GlWidget::~GlWidget()
{
	delete camera;
	delete engine;
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	switch(keyEvent->key())
	{
		case Qt::key_Escape:
			close();
			break;
	}
}

void GlWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//INIT ENGINE 
	/*this->engine = new Engine();

	if (!engine->initSystem())
	{
		QMessageBox msgBox;
		msgBox.setText("Error initializing engine.");
		msgBox.exec();
	}

	if (!engine->initController())
	{
		QMessageBox msgBox;
		msgBox.setText("Error initializing controller for engine.");
		msgBox.exec();
	}
	//create scene
	this->scene = std::make_shared<Scene>();
	//create camera
	this->camera = new Camera(glm::vec3(0.0, 0.0, 8.0), glm::vec3(0.0, 0.0, 0.0));
	//init antweakBar
	this->engine->initAntWeakBar("TOOLS", camera);

	//adding camera to the scene
	scene->setCamera(camera);

	//scene.setCubeMap(pathToTextures + "CubeMap/Skybox");

	scene->addPointLight(glm::vec3(0.0,3.0,8.0), glm::vec3(1.0), glm::vec3(0.1f, 0.8f, 0.8f), 1.0f, 30.0f, 0.0f, "light_1");

	unsigned int error;

	ObjectNode *brique = scene->importModelFromFile(pathToBasicModels + "plan2.obj", "briques");
	brique->setShadowMapped(false);
	DefaultMaterial *parallax = new DefaultMaterial(scene, "parallax_material", &error, 0.02, 0.0);
	//ColorMaterial *parallax = new ColorMaterial(scene, "parallax_material", glm::vec3(0.5, 0.5, 0.5), &error);
	parallax->addTexture("parallax/diffuse.png", scene, SEED_TEXTURE_AMBIANT);
	parallax->addTexture("parallax/normal.png", scene, SEED_TEXTURE_NORMAL);
	parallax->addTexture("parallax/parallax.png", scene, SEED_TEXTURE_DEPTHMAP);
	parallax->scaleModel(glm::vec3(10.0));
	parallax->translateModel(glm::vec3(5.0, -1.0, 0.0));
	//parallax->rotateModel(glm::vec3(0.0,0.0,0.5));
	brique->setMaterialRecur(parallax);
	scene->addNode(brique);

	//collect all the rendered nodes
	scene->collectRenderedNodes();*/
}

void GlWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glTranslatef(3.0f, 0.0f, -6.0f);

    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3d(1.0f, 1.0f, 0.0f);
    glEnd();
	//active loop main render
	//engine->mainRender(scene);
}

void GlWidget::resizeGL(int width, int height)
{
	if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

