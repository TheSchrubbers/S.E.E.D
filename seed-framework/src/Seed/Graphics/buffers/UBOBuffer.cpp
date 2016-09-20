#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <string.h>

UBOBuffer::UBOBuffer()
{
	glGenBuffers(1, &(this->UBOID));
}

UBOBuffer::~UBOBuffer()
{
	glDeleteBuffers(1, &(this->UBOID));
}

void UBOBuffer::create(int s)
{
	this->size = s;
	glBindBuffer(GL_UNIFORM_BUFFER, this->UBOID);
	glBufferData(GL_UNIFORM_BUFFER, s, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBOBuffer::update(void* data, int s)
{
	if (s <= this->size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, this->UBOID);
		GLvoid* pointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(pointer, data, s);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
		//printErrorOpenGL();
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		this->create(s);
		this->update(data, s);
	}
}


void UBOBuffer::destroy()
{
	glDeleteBuffers(1, &(this->UBOID));
}

GLuint UBOBuffer::getID()
{
	return this->UBOID;
}