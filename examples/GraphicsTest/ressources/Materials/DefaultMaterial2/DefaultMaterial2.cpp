#include "DefaultMaterial2.hpp"

DefaultMaterial2::DefaultMaterial2(const aiMaterial *material, Camera *cam, const std::string n, unsigned int *flag) : Material(material, cam, n, flag)
{
	this->M = glm::mat4(1.0);
	glm::translate(M, glm::vec3(-1.0, 0.0, 0.0));
}
DefaultMaterial2::DefaultMaterial2(Camera *cam, const std::string n, unsigned int *flag) : Material(cam, n, "C:/Users/jeremy/Source/Repos/S.E.E.D/examples/GraphicsTest/ressources/Materials/DefaultMaterial2/Shaders", flag)
{
}

DefaultMaterial2::~DefaultMaterial2()
{
}

void DefaultMaterial2::render(Model *model)
{
	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	this->MVP = this->camera->getProjectionMatrix() * camera->getViewMatrix() * this->M;

	// Get a handle for our "MVP" uniform.
	// Only at initialisation time.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	//set the uniform variable MVP
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

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
