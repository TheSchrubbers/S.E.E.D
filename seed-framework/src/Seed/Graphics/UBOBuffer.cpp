#include <Seed/Graphics/UBOBuffer.hpp>

UBOBuffer::UBOBuffer()
{
	glGenBuffers(1, &(this->UBOID));
}

UBOBuffer::~UBOBuffer()
{
	glDeleteBuffers(1, &(this->UBOID));
}

void UBOBuffer::createBuffer(int s)
{
	this->size = s;
	glBindBuffer(GL_UNIFORM_BUFFER, this->UBOID);
	glBufferData(GL_UNIFORM_BUFFER, s, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBOBuffer::updateBuffer(void* data, int s)
{
	if (s <= this->size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, this->UBOID);
		GLvoid* pointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(pointer, data, s);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
		printErrorOpenGL();
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		this->createBuffer(s);
		this->updateBuffer(data, s);
	}
}


void UBOBuffer::deleteBuffer()
{
	glDeleteBuffers(1, &(this->UBOID));
}

GLuint UBOBuffer::getID()
{
	return this->UBOID;
}