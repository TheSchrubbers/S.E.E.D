//SEED INCLUDES
#include <Seed/Graphics/materials/ColorMaterial/ColorMaterial.hpp>
#include <Seed/Graphics/engine/collector.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/shader.hpp>

ColorMaterial::ColorMaterial(std::shared_ptr<Scene> sce, const std::string n, glm::vec3 col, unsigned int *flag, const float reflec, const float refrac) : Material(sce, n, flag, reflec, refrac, pathToMaterials + "ColorMaterial/Shaders")
{
	this->color = col;
	this->init();
}

void ColorMaterial::init()
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
	this->colorID = glGetUniformLocation(programID, "color");
}

ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::render(Model *model)
{
	if (this->activateShader())
	{
		//UNIFORMS
		this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
		//set the uniform variable MVP
		glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
		glUniform1i(this->NMACTIVEID, Scene::normalMappingActive);
		glUniform1i(this->SMACTIVEID, Scene::specularMapActive);
		glUniform1i(this->SMVIEWID, Scene::specularMapView);
		glUniform2f(this->matID, this->mat.Ks, this->mat.Kr);
		glUniform3f(this->colorID, this->color.x, this->color.y, this->color.z);
		//OPTIONS
		//Enable culling triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);
		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//BUFFERS
		for (int i = 0; i < 4; i++)
		{
			//bind UBO buffer light
			glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
			//bind UBO lighting with program shader
			glUniformBlockBinding(this->shader->getID(), this->block_index_lights[i], i);
		}
		//bind UBO buffer camera
		glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 4);
		//RENDER
		//render model
		model->render();

	}
}

void ColorMaterial::translateModel(glm::vec3 T)
{
	this->M = translate(this->M, T);
}

void ColorMaterial::scaleModel(glm::vec3 T)
{
	this->M = scale(this->M, T);
}

void ColorMaterial::rotateModel(glm::vec3 T)
{
	this->M = rotate(this->M, T);
}