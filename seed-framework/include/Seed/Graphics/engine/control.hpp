#ifndef CONTROL_HPP
#define CONTROL_HPP

//OTHER INCLUDES
#include <glm/glm.hpp>

//SEED INCLUDES
#include "Seed/Graphics/engine/Constant.hpp"

class Camera;

/**
 * @brief      Updates the view whenever data changes.
 */
class Controller
{
public:
	
	/**
	 * @brief      Constructor of class Controller
	 *
	 * @param      window  The window
	 */
	Controller();

	/**
	 * @brief      Destructor of class Controller
	 */
	~Controller();
	

	/**
	 * @brief      Update all controls (mouse and keyboard) with GLFW
	 *
	 * @param      window     The window
	 * @param      camera     The camera
	 * @param[in]  deltaTime  The delta time
	 */
	void updateControl(Camera* camera, float deltaTime);

	void updateControlMoveMouse(Camera *cam, float WAngle, float HAngle, double deltaX, double deltaY);

	/**
	 * @brief      Update position of the camera according to the movement of mouse
	 *
	 * @param      cam        The camera
	 * @param[in]  deltaPosX  The delta position x
	 * @param[in]  deltaPosY  The delta position y
	 */
	void updateDragMouseControl(Camera *cam, float deltaPosX, float deltaPosY);
	
	/**
	 * @brief      Initialize AntTweakBar
	 *
	 * @param[in]  name  The name of the AntTweakBar toolbar
	 */
	void initAntWeakBar(std::string name);

private:
	Camera *cam;
};

#endif