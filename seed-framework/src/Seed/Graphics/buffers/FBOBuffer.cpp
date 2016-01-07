#include <Seed/Graphics/Buffers/FBOBuffer.hpp>

FBOBuffer::FBOBuffer()
{
	glGenFramebuffers(1, &(this->FBOID));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBOID);
	//Gposition buffer
	glGenTextures(1, &this->GPosition);
	glBindTexture(GL_TEXTURE_2D, this->GPosition);
	//create texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//parameters
	//we want to get the nearest value which corresponds of th fragment value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//attach the texture position into the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GPosition, 0);
	//GNormal buffer
	glGenTextures(1, &this->GNormal);
	glBindTexture(GL_TEXTURE_2D, this->GNormal);
	//create texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//parameters
	//we want to get the nearest value which corresponds of th fragment value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//attach the texture position into the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GNormal, 0);
	//GSpecColor
	glGenTextures(1, &this->GColorSpec);
	glBindTexture(GL_TEXTURE_2D, this->GColorSpec);
	//create texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	//parameters
	//we want to get the nearest value which corresponds of th fragment value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//attach the texture position into the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GColorSpec, 0);
	//tell to the framebuffer which are the attachments
	attachments[0] = GL_COLOR_ATTACHMENT0;
	attachments[1] = GL_COLOR_ATTACHMENT1;
	attachments[2] = GL_COLOR_ATTACHMENT2;
	glDrawBuffers(3, attachments);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR the FrameBuffer failed" << std::endl;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

FBOBuffer::~FBOBuffer()
{
	glDeleteFramebuffers(1, &(this->FBOID));
}


void FBOBuffer::deleteBuffer()
{
	glDeleteFramebuffers(1, &(this->FBOID));
}

GLuint FBOBuffer::getID()
{
	return this->FBOID;
}

void FBOBuffer::bindWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBOID);
}

void FBOBuffer::bindRead(GLuint programID)
{
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, this->FBOID);
	this->activeTextures(programID);
}

void FBOBuffer::release()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FBOBuffer::activeTextures(GLuint programID)
{
	//active gposition
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->GPosition);
	glUniform1i(glGetUniformLocation(programID, "gPosition"), 0);
	//active gNormal
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->GNormal);
	glUniform1i(glGetUniformLocation(programID, "gNormal"), 1);
	//active gColor&spec
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->GColorSpec);
	glUniform1i(glGetUniformLocation(programID, "gColorSpec"), 2);
}

void FBOBuffer::printTextures()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->FBOID);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH / 2.0f, HEIGHT / 2.0f, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, HEIGHT / 2.0f, WIDTH / 2.0f, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glReadBuffer(GL_COLOR_ATTACHMENT2);
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, WIDTH / 2.0f, HEIGHT / 2.0f, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBOBuffer::releaseTextures()
{
	//active gposition
	glBindTexture(GL_TEXTURE_2D, 0);
}