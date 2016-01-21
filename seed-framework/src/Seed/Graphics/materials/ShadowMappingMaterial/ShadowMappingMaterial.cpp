//SEED INCLUDES
#include <Seed/Graphics/materials/ShadowMappingMaterial/ShadowMappingMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>

ShadowMappingMaterial::ShadowMappingMaterial(std::shared_ptr<Scene> sce, unsigned int *flag) : Material(sce, "ShadowMapping", flag, 0.0, 0.0)
{
	this->shader = std::make_shared<Shader>(pathToMaterials + "ShadowMapping/Shaders", flag);
	if (*flag == SEED_SUCCESS)
		this->init();
	else
		writeLog("ERROR -> Material : ShadowMappingMaterial loading fails");
}

void ShadowMappingMaterial::init()
{
	glm::mat4 V = glm::lookAt(glm::vec3(0.0,3.0,8.0), glm::vec3(0.0), glm::vec3(0.0,1.0,0.0));
	glm::mat4 P = glm::perspective(
		180.0f,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		1.0f,        // la ou commence le frustrum
		30.0f// la ou finit le frustrum
		);
	//this->PLight = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.0, 30.0);
	this->VPLight = P * V;
	this->WVPlightID = glGetUniformLocation(this->shader->getID(), "WVP");
}

ShadowMappingMaterial::~ShadowMappingMaterial()
{
}

void ShadowMappingMaterial::firstPass(Model *model, glm::mat4 &ModelMatrix, glm::mat4 &VP)
{
	
	if (this->shader->useProgram())
	{
		this->WVPLight = VP * ModelMatrix;
		//UNIFORMS
		//set the uniform variable MVP
		glUniformMatrix4fv(this->WVPlightID, 1, GL_FALSE, &(this->WVPLight[0][0]));

		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		//RENDER
		//render model
		model->render();

		glCullFace(GL_BACK);

	}
}