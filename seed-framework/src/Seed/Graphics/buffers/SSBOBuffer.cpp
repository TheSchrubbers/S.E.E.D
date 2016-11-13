#include "Seed/Graphics/buffers/SSBOBuffer.hpp"
#include "Seed/Graphics/particles/SPH.hpp"
#include <string.h>

SSBOBuffer::SSBOBuffer()
{
}

SSBOBuffer::~SSBOBuffer()
{
	glDeleteBuffers(1, &(this->m_id));
}

void SSBOBuffer::createBuffer(int s)
{
	glGenBuffers(1, &(this->m_id));
	this->size = s;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, s, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBOBuffer::updateBuffer(void* data, int s)
{
	if (s <= this->size)
	{
		//bind the SSBO buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_id);
		//get the SSBO buffer
		GLvoid* pointer = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		//write into the SSBO buffer with the data
		memcpy(pointer, data, s);
		//unbind the SSBO buffer
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}


void SSBOBuffer::deleteBuffer()
{
	glDeleteBuffers(1, &(this->m_id));
}

GLuint SSBOBuffer::getID()
{
	return this->m_id;
}

GLvoid* SSBOBuffer::getData(float s)
{
	return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
}

void SSBOBuffer::bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->m_id);
}

void SSBOBuffer::release()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}