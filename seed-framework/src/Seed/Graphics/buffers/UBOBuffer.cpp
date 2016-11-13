#include "Seed/Graphics/buffers/UBOBuffer.hpp"
#include <string.h>

UBOBuffer::UBOBuffer()
{
	glGenBuffers(1, &m_id);
}

UBOBuffer::~UBOBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void UBOBuffer::create(int s)
{
	m_size = s;
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	glBufferData(GL_UNIFORM_BUFFER, s, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBOBuffer::update(void* data, int s)
{
	if (s <= m_size)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
		GLvoid* pointer = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		memcpy(pointer, data, s);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
		//printErrorOpenGL();
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		create(s);
		update(data, s);
	}
}


void UBOBuffer::destroy()
{
	glDeleteBuffers(1, &(m_id));
}

GLuint UBOBuffer::getID()
{
	return m_id;
}