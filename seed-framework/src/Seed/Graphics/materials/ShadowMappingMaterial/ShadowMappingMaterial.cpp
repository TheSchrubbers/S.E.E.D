//SEED INCLUDES
#include <Seed/Graphics/materials/ShadowMappingMaterial/ShadowMappingMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>
#include <Seed/Graphics/buffers/FBOBuffer.hpp>

ShadowMappingMaterial::ShadowMappingMaterial(std::shared_ptr<Scene> sce, unsigned int *flag) : Material(sce, "ShadowMapping", flag, 0.0, 0.0)
{
	this->shader1 = std::make_shared<Shader>(pathToMaterials + "ShadowMapping/Shaders/firstPass", flag);
	//load shaders
	this->shader2 = std::make_shared<Shader>(pathToMaterials + "ShadowMapping/Shaders/secondPass", flag);
	if (*flag == SEED_SUCCESS)
		this->init();
	else
		writeLog("ERROR -> Material : ShadowMappingMaterial loading fails");
}

void ShadowMappingMaterial::init()
{
	/*GLuint programID = this->shader1->getID();

	this->M = glm::mat4(1.0);

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(programID, "M");
	this->NMID = glGetUniformLocation(programID, "Normal_Matrix");
	this->matID = glGetUniformLocation(programID, "mat");
	this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
	this->WVPlightID = glGetUniformLocation(programID, "WVP");
	this->firstPassBoolID = glGetUniformLocation(programID, "firstPass");*/
}

ShadowMappingMaterial::~ShadowMappingMaterial()
{
}

void ShadowMappingMaterial::firstPass(Model *model)
{
	GLuint programID = this->shader1->getID();
	this->M = glm::mat4(1.0);
	glm::mat4 V = glm::lookAt(glm::vec3(0.0,2.0,3.0), glm::vec3(0.0), glm::vec3(0.0,1.0,0.0));
	glm::mat4 P = glm::perspective(
		45.0f,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		1.0f,        // la ou commence le frustrum
		7.5f// la ou finit le frustrum
		);
	//glm::mat4 P = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.0, 7.5);
	this->WVPlight = P * V;
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(programID, "M");
	this->WVPlightID = glGetUniformLocation(programID, "WVP");
	if (this->shader1->useProgram())
	{
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->WVPlightID, 1, GL_FALSE, &(this->WVPlight[0][0]));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		//RENDER
		//render model
		model->render();

	}
}

void ShadowMappingMaterial::secondPass(Model *model)
{
	GLuint programID = this->shader2->getID();
	this->M = glm::mat4(1.0);
	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(programID, "M");
	this->NMID = glGetUniformLocation(programID, "Normal_Matrix");
	this->matID = glGetUniformLocation(programID, "mat");
	/*this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");*/
	//this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");

	if (this->shader2->useProgram())
	{
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		glUniform1i(glGetUniformLocation(programID, "gShadowMap"), 0);

		//OPTIONS
		glDisable(GL_CULL_FACE);

		//BUFFERS
		/*for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader2->getID(), this->block_index_lights[i], i);
		}*/
		//bind UBO buffer camera
		//glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		//glUniformBlockBinding(this->shader2->getID(), this->block_index_camera, 4);
		//RENDER
		//render model
		model->render();
	}
}