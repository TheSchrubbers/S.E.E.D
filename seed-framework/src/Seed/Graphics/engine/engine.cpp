//SEED INCLUDES
#include <Seed/Graphics/engine/engine.hpp>

Engine::Engine()
{
}

Engine::~Engine()
{
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	TwTerminate();
}

void Engine::mainRender(std::shared_ptr<Scene> scene)
{
	double currentTime = 0, lastTime = 0;
	float deltaTime = 0;

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	//main loop to render
	do
	{
		// Clear the depthbuffer and the colourbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//get events glfw
		glfwPollEvents();
		//get current time
		currentTime = glfwGetTime();

		//update mouse control and keyboard control
		this->controller->updateControl(this->window, scene->getCamera(), deltaTime);

		//node->getMaterial()->setLight(a, d, s);
		scene->render();
		//scene->SSAOrender();

		//Draw anttweakbar
		TwDraw();
		//get current time
		lastTime = glfwGetTime();

		//time between 2 frames
		deltaTime = float(lastTime - currentTime);

		//on nettoie les buffers
		glfwSwapBuffers(this->window);

	} while (glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(this->window) == 0);
}
bool Engine::initSystem()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	window = glfwCreateWindow(WIDTH, HEIGHT, "Moteur3d", NULL, NULL);

	if (window == NULL){
		std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW 
	glewExperimental = GL_TRUE; // Needed in core profile 

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW, version of opengl must be greater or equal than opengl 3.2\n" << std::endl;
		return false;
	}
	return true;
}

bool Engine::initController()
{
	this->controller = new Controller(this->window);
	return true;
}

void Engine::initAntWeakBar(std::string name, const Camera* camera)
{
	this->controller->initAntWeakBar(name);

	float a = 0.1f, d = 0.5f, s = 0.4f;
	float deltat = 0.005;
	/*TwAddVarRW(this->controller->getBar(), "Ambiant composant", TW_TYPE_FLOAT, &a, " min=0.0 max=1.0 step=0.01 group=Engine label='Ambiant composant' ");
	TwAddVarRW(this->controller->getBar(), "Diffuse composant", TW_TYPE_FLOAT, &d, " min=0.0 max=1.0 step=0.01 group=Engine label='Diffuse composant' ");
	TwAddVarRW(this->controller->getBar(), "Specular composant", TW_TYPE_FLOAT, &s, " min=0.0 max=1.0 step=0.01 group=Engine label='Specular composant' ");*/
	// Add callback to toggle auto-rotate mode (callback functions are defined above).
	TwAddVarRW(this->controller->getBar(), "WIREFRAME", TW_TYPE_BOOL8, &(Scene::wireframe), " label='WireFrame' help='Active/Desactive mode wireframe' group=Mods");
	TwAddVarRW(this->controller->getBar(), "SPECULARMAPVIEW", TW_TYPE_BOOL8, &(Scene::specularMapView), "label='Specular Mapping View' help='View Specular Mapping' group=Mods");
	TwAddVarRW(this->controller->getBar(), "SPECULARMAPACTIVE", TW_TYPE_BOOL8, &(Scene::specularMapActive), "label='Specular Mapping Active' help='Active/Desactive Specular Mapping' group=Mods");
	TwAddVarRW(this->controller->getBar(), "NORMALMAPPING", TW_TYPE_BOOL8, &(Scene::normalMappingActive), "label='Normal mapping' help='Active/Desactive mode Normal mapping' group=Mods");
	TwAddVarRW(this->controller->getBar(), "DELTAT", TW_TYPE_FLOAT, &(SPH::deltat), " label='deltat' help='Delta t of the animation' min=0.00001 max=0.1 step=0.00001 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "K", TW_TYPE_FLOAT, &(SPH::K), " label='Stiffness K' help='Stiffness K' min=0.1 max=10.0 step=0.1 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "RADIUSPARTICLE", TW_TYPE_FLOAT, &(SPH::radiusParticle), " label='Radius Particle' min=0.01 max=1.0 step=0.05 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "NBPARTICLES", TW_TYPE_FLOAT, &(SPH::nbParticles), " label='Nb particles' min=10 max=1000000 step=10 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "VISCOSITY", TW_TYPE_FLOAT, &(SPH::mu), " label='Viscosity fluid' min=0.01 max=1.0 step=0.01 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "DENSITY", TW_TYPE_FLOAT, &(SPH::densityFluid), " label='Density fluid' min=1.0 max=10000.0 step=1.0 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "RADIUSSPHERESTARTER", TW_TYPE_FLOAT, &(SPH::radiusSphereStarter), " label='Radius sphere starter' min=0.1 max=5.0 step=0.1 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "SURFACETENSION", TW_TYPE_FLOAT, &(SPH::sigma), " label='Surface tension coefficient' min=0.01 max=1.0 step=0.01 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "NEIGHBORSNB", TW_TYPE_FLOAT, &(SPH::x), " label='x particles in volume neighbouring' min=1.0 max=100.0 step=1.0 group=parameters ");
	TwAddVarRW(this->controller->getBar(), "SIZECUBECONTAINER", TW_TYPE_FLOAT, &(SPH::sizeCube), " label='Size of the container cube' min=0.5 max=10.0 step=0.1 group=parameters");
	//TwAddVarRO(this->controller->getBar(), "MASSPARTICLE", TW_TYPE_FLOAT, &(SPH::mass), " label='Mass particle' min=0.001 max=10.0 step=0.01 group=read visible=false");
	TwAddVarRO(this->controller->getBar(), "RADIUSNEIBOURING", TW_TYPE_FLOAT, &(SPH::radiusNeighbouring), " label='Radius Neighbouring' min=0.01 max=1.0 step=0.05 group=read ");
	TwAddVarRO(this->controller->getBar(), "AVERAGENEIGHBOURING", TW_TYPE_FLOAT, &(SPH::AverageNeighbors), " label='Average neighbouring'  group=read ");
	TwAddVarRO(this->controller->getBar(), "NBPARTICLES", TW_TYPE_INT32, &(SPH::nbPart), " label='Current number of particles'  group=read ");
	TwAddVarRW(this->controller->getBar(), "COEFMERGE", TW_TYPE_FLOAT, &(SPH::mergeCoef), " label='Merge' help='Coefficient of merging particle' min=0.001 max=1.0 step=0.001 group=Split-Merge ");
	TwAddVarRW(this->controller->getBar(), "COEFSPLIT", TW_TYPE_FLOAT, &(SPH::splitCoef), " label='Split' help='Coefficient of splitting particle' min=0.1 max=1.0 step=0.001 group=Split-Merge ");
	TwAddButton(this->controller->getBar(), "NEXTFRAME", CallbackButtonReset, NULL, " label='Reset' group=control ");
	TwAddButton(this->controller->getBar(), "PLAY", CallbackButtonPlay, NULL, " label='Play' group=control ");
	TwAddButton(this->controller->getBar(), "PAUSE", CallbackButtonPause, NULL, " label='Pause' group=control ");
	TwAddButton(this->controller->getBar(), "RESET", CallbackButtonNextFrame, NULL, " label='Next frame' group=control ");
	TwAddVarRW(this->controller->getBar(), "HALF", TW_TYPE_BOOL8, &(SPH::half), " label='Half' help='Print half particles' group=control ");
	TwAddVarRW(this->controller->getBar(), "GRAVITY", TW_TYPE_BOOL8, &(SPH::SPHGravity), " label='Gravity' help='EnabledGravity' group=control ");
	TwDefine(" TOOLS/parameters   group=SPH");
	TwDefine(" TOOLS/Split-Merge   group=SPH");
	TwDefine(" TOOLS/read   group=SPH");
	TwDefine(" TOOLS/control   group=SPH");
	//TwAddVarCB(this->controller->getBar(), "MouseSpeed", TW_TYPE_FLOAT, Camera::SetSpeedMouseCallback, Camera::GetSpeedMouseCallback, &camera, "label='Speed camera' help='Set the speed of the orientation of the camera' min=10");

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
