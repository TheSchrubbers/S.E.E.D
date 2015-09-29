#include <DefaultMaterial/DefaultMaterial.hpp>

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

	this->block_index_lights = glGetUniformBlockIndex(programID, "LightsBuffer");
	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
}

DefaultMaterial::~DefaultMaterial()
{
}

void DefaultMaterial::render(Model *model)
{
	this->M *= glm::rotate(0.005f, glm::vec3(0, 1, 0));
	this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
	//set the uniform variable MVP
	glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);
	glUniform1fv(this->compl.ambiantID, 1, &(compl.ambiant));
	glUniform1fv(this->compl.diffuseID, 1, &(compl.diffuse));
	glUniform1fv(this->compl.specularID, 1, &(compl.specular));

	int i = 0;

	//Enable culling triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);


	int nbTextures = this->textures.size();
	//active and bind textures
	for (i = 0; i < nbTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		this->textures[i]->bind();
	}

	//bind UBO buffer light
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->scene->getLightUBO()->getID());
	//bind UBO lighting with program shader
	glUniformBlockBinding(this->programID, this->block_index_lights, 0);

	//bind UBO buffer camera
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->scene->getCamUBO()->getID());
	//bind UBO camera with program shader
	glUniformBlockBinding(this->programID, this->block_index_camera, 1);

	//render model
	model->render();

	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);

	//release textures
	for (i = 0; i < nbTextures; i++)
	{
		this->textures[i]->release();
	}
}