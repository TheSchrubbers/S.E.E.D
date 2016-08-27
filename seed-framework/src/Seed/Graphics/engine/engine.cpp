//SEED INCLUDES
#include <Seed/Graphics/engine/engine.hpp>

using namespace std;

Engine::Engine()
{
	initLog();
}

Engine::~Engine()
{
	// Close OpenGL window and terminate GLFW
	//glfwDestroyWindow(m_window);
	//glfwTerminate();

	//TwTerminate();
}

shared_ptr<Scene> Engine::createScene()
{
	//create scene
	m_scene = std::make_shared<Scene>();
	return m_scene;
}

vector<ObjectNode*> Engine::loadSystemToRendering(shared_ptr<Scene> scene)
{
	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;
	vector<PointLightNode*> *pointLightNodes;
	vector<FlashLightNode*> *flashLightNodes;
	vector<DirectionnalLightNode*> *  directionnalLightNodes;
	vector<SpotLightNode*> * spotLightNodes;
	stack<ObjectNode*> n;
	vector<ObjectNode*> nodes;
	ObjectNode *n2 = nullptr;

	//glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	//get all the nodes with a model
	n.push(scene->getRootObjectNode());
	while (!n.empty())
	{
		n2 = n.top();
		n.pop();
		for (int i = 0; i < n2->getChildren()->size(); i++)
			n.push(n2->getChildren()->at(i));
		if (n2->getModel())
			nodes.push_back(n2);
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//set FBO buffer textures for shadow mapping
	pointLightNodes = scene->getCollector()->getPointLightNodes();
	for (int i = 0; i < pointLightNodes->size(); i++)
		if (pointLightNodes->at(i)->getLight()->isSendShadow())
			scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	directionnalLightNodes = scene->getCollector()->getDirectionnalLightNodes();
	for (int i = 0; i < directionnalLightNodes->size(); i++)
		if (directionnalLightNodes->at(i)->getLight()->isSendShadow())
			scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	spotLightNodes = scene->getCollector()->getSpotLightNodes();
	for (int i = 0; i < spotLightNodes->size(); i++)
		if (spotLightNodes->at(i)->getLight()->isSendShadow())
			scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);
	flashLightNodes = scene->getCollector()->getFlashLightNodes();
	for (int i = 0; i < flashLightNodes->size(); i++)
		if (flashLightNodes->at(i)->getLight()->isSendShadow())
			scene->getFBOBuffer()->createTexture(GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

	pointLightNodes = nullptr;
	spotLightNodes = nullptr;
	directionnalLightNodes = nullptr;
	flashLightNodes = nullptr;
	n2 = nullptr;

	return nodes;
}
void Engine::mainRender(std::shared_ptr<Scene> scene, vector<ObjectNode*> nodes)
{
	// main loop to render
	//do
	//{
		// Clear the depthbuffer and the colourbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		//get current time
		//currentTime = glfwGetTime();

		//update mouse control and keyboard control
		//m_controller->updateControl(this->window, scene->getCamera(), deltaTime);

		scene->render(nodes);

		//Draw anttweakbar
		//TwDraw();
		//get current time
		//lastTime = glfwGetTime();

		//time between 2 frames
		//deltaTime = float(lastTime - currentTime);

		//on nettoie les buffers
		//glfwSwapBuffers(this->window);

		//get events glfw
		//glfwPollEvents();

	//} while(1);//while (glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(this->window) == 0);

}
bool Engine::initGLFWSystem(const int w, const int h)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		writeLog("Failed to initialize GLFW");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	m_window = glfwCreateWindow(w, h, "Moteur3d", nullptr, nullptr);

	if (m_window == nullptr){
		writeLog("Failed to open GLFW window. Not 4.3 compatible.");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window); // Initialize GLEW 
	glfwSwapInterval(1);

	return true;
}
bool Engine::initGlewSystem()
{
	//Initialize Glew system
	glewExperimental = GL_TRUE; // Needed in core profile 

	if (glewInit() != GLEW_OK) {
		writeLog("Failed to initialize GLEW, version of opengl must be greater or equal than opengl 4.3");
		return false;
	}

	return true;
}

bool Engine::initController()
{
	//initialize Controller to update viewing camera
	m_controller = new Controller(m_window);
	return true;
}

void Engine::updateDragMouseControl(float dragPosX, float dragPosY)
{
	m_controller->updateDragMouseControl(m_scene->getCamera(), dragPosX, dragPosY);
}

void Engine::initAntWeakBar(std::string name, const Camera* camera)
{
	m_controller->initAntWeakBar(name);

	float a = 0.1f, d = 0.5f, s = 0.4f;
	float deltat = 0.005;
	/*TwAddVarRW(m_controller->getBar(), "Ambiant composant", TW_TYPE_FLOAT, &a, " min=0.0 max=1.0 step=0.01 group=Engine label='Ambiant composant' ");
	TwAddVarRW(m_controller->getBar(), "Diffuse composant", TW_TYPE_FLOAT, &d, " min=0.0 max=1.0 step=0.01 group=Engine label='Diffuse composant' ");
	TwAddVarRW(m_controller->getBar(), "Specular composant", TW_TYPE_FLOAT, &s, " min=0.0 max=1.0 step=0.01 group=Engine label='Specular composant' ");*/
	// Add callback to toggle auto-rotate mode (callback functions are defined above).
	TwAddVarRW(m_controller->getBar(), "WIREFRAME", TW_TYPE_BOOL8, &(Scene::wireframe), " label='WireFrame' help='Active/Desactive mode wireframe' group=Mods");
	TwAddVarRW(m_controller->getBar(), "SPECULARMAPVIEW", TW_TYPE_BOOL8, &(Scene::specularMapView), "label='View specularmap' help='View Specular Map' group=SpecularMap");
	TwAddVarRW(m_controller->getBar(), "SPECULARMAPACTIVE", TW_TYPE_BOOL8, &(Scene::specularMapActive), "label='Active specularmap' help='Active/Desactive Specular Map' group=SpecularMap");
	TwAddVarRW(m_controller->getBar(), "NORMALMAPPVIEW", TW_TYPE_BOOL8, &(Scene::normalMapView), "label='View normalmap' help='View Normalmap' group=NormalMap");
	TwAddVarRW(m_controller->getBar(), "NORMALMAPACTIVE", TW_TYPE_BOOL8, &(Scene::normalMapActive), "label='Active normalmap' help='Active/Desactive mode Normalmap' group=NormalMap");
	TwAddVarRW(m_controller->getBar(), "HEIGHTMAPVIEW", TW_TYPE_BOOL8, &(Scene::parallaxMapView), "label='View heightmap' help='View heightmap' group=HeightMap");
	TwAddVarRW(m_controller->getBar(), "HEIGHTMAPACTIVE", TW_TYPE_BOOL8, &(Scene::parallaxMapActive), "label='Active heightmap' help='Active/Desactive mode heightmap' group=HeightMap");
	TwAddVarRW(m_controller->getBar(), "BIASPARALLAX", TW_TYPE_FLOAT, &(Scene::biasParallax), " label='Bias parallax mapping' help='Bias of parallaxMap' min=0.1 max=1.0 step=0.1 group=HeightMap ");

	//SPH
	TwAddVarRW(m_controller->getBar(), "DELTAT", TW_TYPE_FLOAT, &(SPH::deltat), " label='deltat' help='Delta t of the animation' min=0.00001 max=0.1 step=0.00001 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "K", TW_TYPE_FLOAT, &(SPH::K), " label='Stiffness K' help='Stiffness K' min=0.1 max=10.0 step=0.1 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "RADIUSPARTICLE", TW_TYPE_FLOAT, &(SPH::radiusParticle), " label='Radius Particle' min=0.01 max=1.0 step=0.05 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "NBPARTICLES", TW_TYPE_FLOAT, &(SPH::nbParticles), " label='Nb particles' min=10 max=1000000 step=10 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "VISCOSITY", TW_TYPE_FLOAT, &(SPH::mu), " label='Viscosity fluid' min=0.01 max=1.0 step=0.01 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "DENSITY", TW_TYPE_FLOAT, &(SPH::densityFluid), " label='Density fluid' min=1.0 max=10000.0 step=1.0 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "RADIUSSPHERESTARTER", TW_TYPE_FLOAT, &(SPH::radiusSphereStarter), " label='Radius sphere starter' min=0.1 max=5.0 step=0.1 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "SURFACETENSION", TW_TYPE_FLOAT, &(SPH::sigma), " label='Surface tension coefficient' min=0.01 max=1.0 step=0.01 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "NEIGHBORSNB", TW_TYPE_FLOAT, &(SPH::x), " label='x particles in volume neighbouring' min=1.0 max=100.0 step=1.0 group=parameters ");
	TwAddVarRW(m_controller->getBar(), "SIZECUBECONTAINER", TW_TYPE_FLOAT, &(SPH::sizeCube), " label='Size of the container cube' min=0.5 max=10.0 step=0.1 group=parameters");
	//TwAddVarRO(m_controller->getBar(), "MASSPARTICLE", TW_TYPE_FLOAT, &(SPH::mass), " label='Mass particle' min=0.001 max=10.0 step=0.01 group=read visible=false");
	TwAddVarRO(m_controller->getBar(), "RADIUSNEIBOURING", TW_TYPE_FLOAT, &(SPH::radiusNeighbouring), " label='Radius Neighbouring' min=0.01 max=1.0 step=0.05 group=read ");
	TwAddVarRO(m_controller->getBar(), "AVERAGENEIGHBOURING", TW_TYPE_FLOAT, &(SPH::AverageNeighbors), " label='Average neighbouring'  group=read ");
	TwAddVarRO(m_controller->getBar(), "NBPARTICLES", TW_TYPE_INT32, &(SPH::nbPart), " label='Current number of particles'  group=read ");
	TwAddVarRW(m_controller->getBar(), "COEFMERGE", TW_TYPE_FLOAT, &(SPH::mergeCoef), " label='Merge' help='Coefficient of merging particle' min=0.001 max=1.0 step=0.001 group=Split-Merge ");
	TwAddVarRW(m_controller->getBar(), "COEFSPLIT", TW_TYPE_FLOAT, &(SPH::splitCoef), " label='Split' help='Coefficient of splitting particle' min=0.1 max=1.0 step=0.001 group=Split-Merge ");
	TwAddButton(m_controller->getBar(), "NEXTFRAME", CallbackButtonReset, NULL, " label='Reset' group=control ");
	TwAddButton(m_controller->getBar(), "PLAY", CallbackButtonPlay, NULL, " label='Play' group=control ");
	TwAddButton(m_controller->getBar(), "PAUSE", CallbackButtonPause, NULL, " label='Pause' group=control ");
	TwAddButton(m_controller->getBar(), "RESET", CallbackButtonNextFrame, NULL, " label='Next frame' group=control ");
	TwAddVarRW(m_controller->getBar(), "HALF", TW_TYPE_BOOL8, &(SPH::half), " label='Half' help='Print half particles' group=control ");
	TwAddVarRW(m_controller->getBar(), "GRAVITY", TW_TYPE_BOOL8, &(SPH::SPHGravity), " label='Gravity' help='EnabledGravity' group=control ");
	//Shadow map
	TwAddVarRW(m_controller->getBar(), "SHADOWMAP", TW_TYPE_BOOL8, &(Scene::shadowMapActive), " label='ShadowMap' help='Active/Desactive shadowmap' group=ShadowMap");
	TwAddVarRW(m_controller->getBar(), "BIAS", TW_TYPE_FLOAT, &(Scene::bias), " label='Bias' help='Bias of shadowMap' min=0.0005 max=0.005 step=0.0001 group=ShadowMap ");

	TwDefine(" TOOLS/parameters   group=SPH");
	TwDefine(" TOOLS/Split-Merge   group=SPH");
	TwDefine(" TOOLS/read   group=SPH");
	TwDefine(" TOOLS/control   group=SPH");
	TwDefine(" TOOLS/SpecularMap	group=Mods");
	TwDefine(" TOOLS/NormalMap	group=Mods");
	TwDefine(" TOOLS/HeightMap 	group=Mods");
	TwDefine(" TOOLS/ShadowMap 	group=Mods");

	//TwAddVarCB(m_controller->getBar(), "MouseSpeed", TW_TYPE_FLOAT, Camera::SetSpeedMouseCallback, Camera::GetSpeedMouseCallback, &camera, "label='Speed camera' help='Set the speed of the orientation of the camera' min=10");

}

void TW_CALL CallbackButtonReset(void *clientData)
{
	SPH::reset = true;
}

void TW_CALL CallbackButtonNextFrame(void *clientData)
{
	SPH::nextFrame = true;
}

void TW_CALL CallbackButtonPlay(void *clientData)
{
	SPH::play = true;
}

void TW_CALL CallbackButtonPause(void *clientData)
{
	SPH::play = false;
}

shared_ptr<Scene> Engine::getScene()
{
	return m_scene;
}

Controller* Engine::getController()
{
	return m_controller;
}