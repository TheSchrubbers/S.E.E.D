/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file engine.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef ENGINE_HPP
#define ENGINE_HPP

//DEFINES
//#define GLFW_INCLUDE_GLCOREARB
//#define GLFW_INCLUDE_GLU
//
//OTHER INCLUDES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
//SEED INCLUDES
#include <Seed/Graphics/engine/control.hpp>
#include <Seed/Graphics/engine/scene.hpp>
#include <Seed/Graphics/materials/DefaultMaterial/DefaultMaterial.hpp>
#include <Seed/Graphics/materials/SSAOMaterial/DeferredMaterial/DeferredMaterial.hpp>
#include <Seed/Graphics/materials/ShadowMappingMaterial/ShadowMappingMaterial.hpp>
#include <Seed/Graphics/node/objectNode.hpp>
#include <Seed/Graphics/materials/ImplicitMaterial/ImplicitMaterial.hpp>
#include <Seed/Graphics/materials/ColorMaterial/ColorMaterial.hpp>
#include <Seed/Graphics/particles/SPH.hpp>
#include <Seed/Graphics/materials/ParticlesSystemMaterial/SPHMaterial/SPHMaterial.hpp>
#include <Seed/Graphics/engine/tools.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/node/pointLightNode.hpp>
#include <Seed/Graphics/node/directionalLightNode.hpp>
#include <Seed/Graphics/node/spotLightNode.hpp>
#include <Seed/Graphics/node/flashLightNode.hpp>
#include <Seed/Graphics/lights/directionnalLight.hpp>
#include <Seed/Graphics/lights/flashLight.hpp>
#include <Seed/Graphics/lights/pointLight.hpp>
#include <Seed/Graphics/lights/spotLight.hpp>
#include <Seed/Graphics/engine/camera.hpp>


/*! \class Engine
* \brief class Engine, init the system and the scene
*/
class Engine
{
	public:
		/*!
		* \brief Constructor of class Scene
		*/
		Engine();
		/*!
		* \brief Destructor of class Node
		*/
		~Engine();

		/**
		 * @brief      Creates a scene.
		 *
		 * @return     The created scene
		 */
		std::shared_ptr<Scene> createScene();

		/**
		 * @brief      Render the scene
		 *
		 * @param[in]  scene  The scene
		 * @param[in]  nodes  The nodes
		 */
		void mainRender(std::shared_ptr<Scene> scene, std::vector<ObjectNode*> nodes);
		
		/**
		 * @brief      Init GLEW system
		 *
		 * @return     True if the GLEW system was initialized and false if a problem was appeared
		 */
		bool initGlewSystem();

		/**
		 * @brief      Init GLFW system
		 *
		 * @param[in]  w     Width of the rendering window
		 * @param[in]  h     Height of the rendering window
		 *
		 * @return     True if the GLFW system was initialized and false if a
		 *             problem was appeared
		 */
		bool initGLFWSystem(const int w, const int h);

		/**
		 * @brief      Initialize Controller
		 *
		 * @return     True if Controller was initalized and false if a problem was appeared
		 */
		bool initController();

		/**
		 * @brief      Calls updateDragMouseControl function of class Controller to update viewing camera
		 *
		 * @param[in]  dragPosX  The drag position x
		 * @param[in]  dragPosY  The drag position y
		 */
		void updateDragMouseControl(float dragPosX, float dragPosY);

		/**
		 * @brief      Loads the scene to rendering.
		 *
		 * @param[in]  scene  The scene
		 *
		 * @return     Returns a vector of ObjectNode* to rendering
		 */
		std::vector<ObjectNode*> loadSystemToRendering(std::shared_ptr<Scene> scene);

		/**
		 * @brief      Init AntTweakBar
		 *
		 * @param[in]  name    The name of the interface AntTweakBar
		 * @param[in]  camera  The camera
		 */
		void initAntWeakBar(std::string name, const Camera *camera);

		/**
		 * @brief      Gets the scene.
		 *
		 * @return     The scene.
		 */
		std::shared_ptr<Scene> getScene();

		Controller* getController();


	private:
		GLFWwindow *m_window;
		Controller *m_controller;
		int width, height;
		std::shared_ptr<Scene> m_scene;
};

void mouse_buttonID_callback(GLFWwindow* window, int button, int action, int mods);
void TW_CALL CallbackButtonReset(void *clientData);
void TW_CALL CallbackButtonNextFrame(void *clientData);
void TW_CALL CallbackButtonPlay(void *clientData);
void TW_CALL CallbackButtonPause(void *clientData);
#endif