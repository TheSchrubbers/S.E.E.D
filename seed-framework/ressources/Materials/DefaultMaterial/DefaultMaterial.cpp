#include <DefaultMaterial/DefaultMaterial.hpp>
#include <Seed/Graphics/collector.hpp>
#include <Seed/Graphics/UBOBuffer.hpp>
DefaultMaterial::DefaultMaterial(const aiMaterial *material, Scene *sce, const std::string n, unsigned int *flag) : Material(material, sce, n, flag)
{
	this->init();
}
DefaultMaterial::DefaultMaterial(Scene *sce, const std::string n, unsigned int *flag) : Material(sce, n, "C:/Users/jeremy/Source/Repos/S.E.E.D/seed-framework/ressources/Materials/DefaultMaterial/Shaders", flag)
{
	this->init();
}

void DefaultMaterial::init()
{
	this->compl.ambiant = 0.1;
	this->compl.diffuse = 0.5;
	this->compl.specular = 0.4;

	this->M = glm::mat4(1.0);

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->MID = glGetUniformLocation(programID, "M");
	this->NMID = glGetUniformLocation(programID, "Normal_Matrix");
	this->compl.ambiantID = glGetUniformLocation(programID, "light.ambiant");
	this->compl.diffuseID = glGetUniformLocation(programID, "light.diffuse");
	this->compl.specularID = glGetUniformLocation(programID, "light.specular");

	this->block_index_lights[0] = glGetUniformBlockIndex(programID, "PointLightsBuffer");
	this->block_index_lights[1] = glGetUniformBlockIndex(programID, "SpotLightsBuffer");
	this->block_index_lights[2] = glGetUniformBlockIndex(programID, "DirectionnalLightsBuffer");
	this->block_index_lights[3] = glGetUniformBlockIndex(programID, "FlashLightsBuffer");
	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
}

DefaultMaterial::~DefaultMaterial()
{
}

void DefaultMaterial::render(Model *model)
{

	//UNIFORMS
	//this->M *= glm::rotate(0.005f, glm::vec3(0, 1, 0));
	this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
	//set the uniform variable MVP
	glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
	glUniform1fv(this->compl.ambiantID, 1, &(compl.ambiant));
	glUniform1fv(this->compl.diffuseID, 1, &(compl.diffuse));
	glUniform1fv(this->compl.specularID, 1, &(compl.specular));

	//OPTIONS
	//Enable culling triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//TEXTURES
	this->activeTextures();

	//BUFFERS
	for (int i = 0; i < 4; i++)
	{
		//bind UBO buffer light
		glBindBufferBase(GL_UNIFORM_BUFFER, i, this->scene->getCollector()->getLightUBO(i)->getID());
		//bind UBO lighting with program shader
		glUniformBlockBinding(this->programID, this->block_index_lights[i], i);
	}

	//bind UBO buffer camera
	glBindBufferBase(GL_UNIFORM_BUFFER, 4, this->scene->getCamUBO()->getID());
	//bind UBO camera with program shader
	glUniformBlockBinding(this->programID, this->block_index_camera, 4);

	//RENDER
	//render model
	model->render();

	//RELEASE
	this->releaseTextures();
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);

}