//SEED INCLUDES
#include <Seed/Graphics/materials/SSAOMaterial/DeferredMaterial/DeferredMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>

DeferredMaterial::DeferredMaterial(const aiMaterial *material, std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(material, sce, n, flag, reflec, refrac)
{
	this->init();
}
DeferredMaterial::DeferredMaterial(std::shared_ptr<Scene> sce, const std::string n, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac, pathToMaterials + "DeferredMaterial/Shaders")
{
	this->init();
}

void DeferredMaterial::init()
{
	GLuint programID = this->shader->getID();

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
	this->NMACTIVEID = glGetUniformLocation(programID, "NormalMappingActive");
	this->SMACTIVEID = glGetUniformLocation(programID, "SpecularMappingActive");
	this->SMVIEWID = glGetUniformLocation(programID, "SpecularMappingView");
}

DeferredMaterial::~DeferredMaterial()
{
}

void DeferredMaterial::render(Model *model)
{
	//TEXTURES
	if (this->activateShader())
	{

		//OPTIONS
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		//glEnable(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		//Enable depth test
		glDepthMask(GL_TRUE);
		
		glDisable(GL_BLEND);
		
		
		//this->activeTextures(this->shader->getID());
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
		glUniform1i(this->NMACTIVEID, Scene::normalMappingActive);
		glUniform1i(this->SMACTIVEID, Scene::specularMapActive);
		glUniform1i(this->SMVIEWID, Scene::specularMapView);
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 4);
		//RENDER
		//render model
		model->render();
		//RELEASE
		//this->releaseTextures();
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		this->shader->release();
	}
}

void DeferredMaterial::translateModel(glm::vec3 T)
{
	this->M = translate(this->M, T);
}

void DeferredMaterial::scaleModel(glm::vec3 T)
{
	this->M = scale(this->M, T);
}

void DeferredMaterial::rotateModel(glm::vec3 T)
{
	this->M = rotate(this->M, T);
}