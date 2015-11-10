#include "DefaultMaterial2.hpp"
#include <Seed/Graphics/Buffers/UBOBuffer.hpp>

DefaultMaterial2::DefaultMaterial2(Scene *sce, const std::string n, const float reflec, const float refrac, unsigned int *flag) : Material(sce, n, reflec, refrac,  "ressources/Materials/DefaultMaterial2/Shaders", flag)
{
	this->M = glm::mat4(1.0);

	this->MID = glGetUniformLocation(this->shader->getID(), "M");
	this->NMID = glGetUniformLocation(this->shader->getID(), "Normal_Matrix");

	this->block_index_camera = glGetUniformBlockIndex(this->shader->getID(), "CameraBuffer");
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
	glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 1);

	//render model
	model->render();

}
