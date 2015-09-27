#include <Seed/Graphics/SSBOBuffer.hpp>

SSBOBuffer::SSBOBuffer()
{
	glGenBuffers(1, &(this->SSBOID));
}

SSBOBuffer::~SSBOBuffer()
{
	glDeleteBuffers(1, &(this->SSBOID));
}

void SSBOBuffer::createBuffer(int s)
{
	this->size = s;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->SSBOID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, s, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBOBuffer::updateBuffer(void* data, int s)
{
	if (s <= this->size)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->SSBOID);
		GLvoid* pointer = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		memcpy(pointer, data, s);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}


void SSBOBuffer::deleteBuffer()
{
	glDeleteBuffers(1, &(this->SSBOID));
}

GLuint SSBOBuffer::getID()
{
	return this->SSBOID;
}