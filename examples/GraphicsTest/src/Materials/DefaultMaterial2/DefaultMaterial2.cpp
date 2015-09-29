#include "DefaultMaterial2.hpp"

DefaultMaterial2::DefaultMaterial2(Scene *sce, const std::string n, unsigned int *flag) : Material(sce, n, "C:/Users/jeremy/Documents/Seed-Graphics/Release/ressources/Materials/DefaultMaterial2/Shaders", flag)
{
	this->M = glm::mat4(1.0);

	this->MID = glGetUniformLocation(programID, "M");
	this->NMID = glGetUniformLocation(programID, "Normal_Matrix");

	this->block_index_camera = glGetUniformBlockIndex(programID, "CameraBuffer");
}

DefaultMaterial2::~DefaultMaterial2()
{
}

void DefaultMaterial2::render(Model *model)
{
	this->Normal_Matrix = glm::transpose(glm::inverse(this->M));
	//set the uniform variable MVP
	glUniformMatrix4fv(this->MID, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(this->NMID, 1, GL_FALSE, &Normal_Matrix[0][0]);

	//Enable culling triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	//bind UBO buffer camera
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->scene->getCamUBO()->getID());
	//bind UBO camera with program shader
	glUniformBlockBinding(this->programID, this->block_index_camera, 1);

	//render model
	model->render();

}
