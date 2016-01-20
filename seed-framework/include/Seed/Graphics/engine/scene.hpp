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
* \file scene.hpp
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <queue>

class Texture;
class Model;
class Material;
class Camera;
class Collector;
class Node;
class ObjectNode;
class PointLightNode;
class UBOBuffer;
class CubeMap;
class QuadMaterial;
class FBOBuffer;
class AssimpLoader;
class ShadowMappingMaterial;

/*! \class Scene
* \brief class Scene who contains meshes, materials...
*/
class Scene
{
	public:
		/*!
		* \brief Constructor of class Scene
		*/
		Scene();
		/*!
		* \brief Destructor of class Node
		*/
		~Scene();
		/*!
		* \brief Importing a model like obj, 3dmax...
		* \param path: path to the file imported
		* \param name: name of the node
		*/
		ObjectNode* importModelFromFile(const std::string path, const std::string name);
		/*!
		* \brief Get the rootnode of the scene
		*/
		ObjectNode* getRootObjectNode();
		/*!
		* \brief Get camera of the scene
		* \return camera of the scene
		*/
		Camera* getCamera();
		/*!
		* \brief Set camera in the scene
		* \param camera: camera adding in the scene
		*/
		void setCamera(Camera *camera);
		/*!
		* \brief Get the objectnode in the tree of objectnodes of the scene
		* \param: name of the node
		* \return the node searched
		*/
		ObjectNode* getObjectNode(const std::string name);
		/*!
		* \brief add node to the scene
		* \param node: node or hierarchy of node
		*/
		void addNode(ObjectNode *node);
		/*!
		* \brief Adding pointLight to the scene
		* \param position: position of the pointlight in the scene
		* \param color: color of the pointlight
		* \param distance: distance of the field of light
		* \param name of the pointlight
		*/
		void addPointLight(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &K, int distance = 50, std::string name = "PointLight");
		/*!
		* \brief Adding spotLight to the scene
		* \param position: position of the spotlight in the scene
		* \param direction: direction of the spotLight
		* \param color: color of the spotlight
		* \param angle: opening angle of the spotlight
		* \param distance: distance of the field of light
		* \param name of the spotlight
		*/
		void addSpotLight(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &color, const glm::vec3 &K, int angle = 30,  int distance = 50, std::string name = "SpotLight");
		/*!
		* \brief Adding directionnalLight to the scene
		* \param color: color of the directionnallight
		* \param direction: direction of the light
		* \param name of the directionnallight
		*/
		void addDirectionnalLight(const glm::vec3 &color, const glm::vec3 &direction, const glm::vec3 &K, std::string name = "DirectionnalLight");
		/*!
		* \brief Adding flashLight to the scene
		* \param position: position of the flashlight in the scene
		* \param direction: direction of the flashLight
		* \param color: color of the flashlight
		* \param distance: distance of the field of light
		* \param name of the flashlight
		*/
		void addFlashLight(const glm::vec3 &position, const glm::vec3 &direction, glm::vec3 &color, const glm::vec3 &K, int distance = 50, std::string name = "FlashLight");
		/*!
		* \brief Adding a Water system particles
		* \param position: position of starter of the particles
		* \param typeShape: shape of the system of particles (CIRCLE, POINT, SPHERE...)
		* \param name of the system of particles
		*/
		void addWaterSystemParticles(const glm::vec3 &positionStarter, const int &typeShape, const int &nb, unsigned int *flag = nullptr, std::string name = "WaterSystemParticles");
		/*!
		* \brief Put the lights in the UBO BUffer
		*/
		void lightsRender();
		/*!
		* \brief collect all the rendered nodes
		*/
		void collectRenderedNodes();
		/*!
		* \brief get the collector
		* \return the collector of the scene
		*/
		Collector* getCollector();

		/*
		* \brief construct cubemap for the scene
		* \param pathDir : path the the dir which contains images top, bottom, right, left, front, back
		*/
		bool setCubeMap(std::string pathDir);

		/*!
		* \brief get the cubemap
		*/
		CubeMap* getCubeMap();

		/*!
		*\brief Render nodes who be must rendered
		*/
		void render(std::vector<ObjectNode*> nodes);
		/*!
		* \brief Render SSAO
		*/
		void SSAOrender();
		void ShadowMappingRender(std::vector<ObjectNode*> nodes);
		//void constructShadowMap();
		FBOBuffer* getFBOBuffer();
		void afficher();

		static bool wireframe, specularMapActive, specularMapView, normalMappingActive, shadowMapActive;
		static float bias;
		
	private:

		ObjectNode *rootObjectNode;
		Node *rootLightNode;
		Camera *camera;
		Collector *collector;
		CubeMap * cubemap;
		Model *RenderingQuad;
		QuadMaterial *RenderingQuadMaterial;
		//FBOBuffer* FBObuffer;
		AssimpLoader * assimpLoader;

		//SHADOW MAPPING
		ShadowMappingMaterial *shadowMappingMaterial;
		FBOBuffer* FBObuffer;
		
		/*!
		* \brief get the number of meshes and materials, build an tree of nodes
		* \param pScene: The assimp scene object
		* \param path : The path to the model file
		*/
		//ObjectNode* loadObjectInScene(const aiScene *pScene, const std::string path, const std::string name);
		/*!
		* \brief Build tree of nodes of the model added
		* \param nodeFather: an ainode object of the tree of nodes
		* \param node: an objectNode object of the tree of objectnodes
		*/
		//void insertRecurNode(const aiScene *pScene, const aiNode *nodeFather, ObjectNode *father);
		/*! 
		* \brief Load meshes in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		* \param path: path name of the mesh importing
		*/
		//void loadMeshes(const aiScene *pScene, std::string path);
		/*!
		* \brief Load materials in an array and push its in the GPU memory
		* \param pScene: Address of scene of Assimp structure
		* \param name: name of the material
		*/
		//void loadMaterials(const aiScene *pScene, std::string name);
		/*!
		* \brief Construct Quad for deferred shading
		*/
		void constructQuad();
};

#endif