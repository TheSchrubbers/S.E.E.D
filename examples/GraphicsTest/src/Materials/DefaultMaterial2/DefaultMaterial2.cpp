#include "DefaultMaterial2.hpp"

DefaultMaterial2::DefaultMaterial2(Scene *sce, const std::string n, unsigned int *flag) : Material(sce, n, "C:/Users/jeremy/Documents/Seed-Graphics/Release/ressources/Materials/DefaultMaterial2/Shaders", flag)
{
	//this->M = glm::mat4(1.0);
	//this->M = glm::translate(M, glm::vec3(-1.5, 0.0, 0.0));

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	this->matrixID = glGetUniformLocation(programID, "MVP");
	this->M = glm::mat4(1.0);
}

DefaultMaterial2::~DefaultMaterial2()
{
}

void DefaultMaterial2::render(Model *model)
{

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	this->MVP = this->camera->getProjectionMatrix() * camera->getViewMatrix() * this->M;

	//set the uniform variable MVP
	glUniformMatrix4fv(this->matrixID, 1, GL_FALSE, &(MVP[0][0]));

	int i = 0;

	int nbTextures = this->textures.size();
	//active and bind textures
	for (i = 0; i < nbTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		this->textures[i]->bind();
	}
	//render model
	model->render();

	//release textures
	for (i = 0; i < nbTextures; i++)
	{
		this->textures[i]->release();
	}
}
