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
	this->VLight = glm::lookAt(glm::vec3(0.0,3.0,8.0), glm::vec3(0.0), glm::vec3(0.0,1.0,0.0));
	/*this->PLight = glm::perspective(
		45.0f,         //angle d'ouverture de la caméra
		(float)WIDTH/(float)HEIGHT, // ratio de la résolution de l'ecran
		1.0f,        // la ou commence le frustrum
		100.0f// la ou finit le frustrum
		);*/
	this->PLight = glm::ortho(-10.0, 10.0, -10.0, 10.0, 1.0, 20.0);
	this->VPLight = this->PLight * this->VLight;
	GLuint programID = this->shader1->getID();
	

	this->WVPlightID = glGetUniformLocation(programID, "WVP");

	programID = this->shader2->getID();
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
	this->VlightID = glGetUniformLocation(programID, "VLight");
}

ShadowMappingMaterial::~ShadowMappingMaterial()
{
}

void ShadowMappingMaterial::firstPass(Model *model, glm::mat4 &ModelMatrix)
{
	
	if (this->shader1->useProgram())
	{
		this->WVPLight = this->VPLight * ModelMatrix;
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(ModelMatrix));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->WVPlightID, 1, GL_FALSE, &(this->WVPLight[0][0]));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void ShadowMappingMaterial::secondPass(Model *model, glm::mat4 &ModelMatrix)
{
	if (this->shader2->useProgram())
	{
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(ModelMatrix));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
		glUniformMatrix4fv(this->VlightID, 1, GL_FALSE, &(this->VPLight[0][0]));
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		glUniform1i(glGetUniformLocation(this->shader2->getID(), "gShadowMap"), 0);

		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader2->getID(), this->block_index_lights[i], i);
		}
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader2->getID(), this->block_index_camera, 4);

		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		//	glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);


		//RENDER
		//render model
		model->render();
	}
}