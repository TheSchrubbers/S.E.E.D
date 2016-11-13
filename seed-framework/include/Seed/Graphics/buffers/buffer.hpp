#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Seed/Graphics/definitions.hpp"

class Buffer : public openGLFunctions
{
public:
	explicit Buffer() : openGLFunctions()
	{
    	initializeOpenGLFunctions();
	}
	~Buffer(){}
	virtual GLuint getID() = 0;
protected:
	GLuint m_id;
};


#endif // BUFFER_HPP
