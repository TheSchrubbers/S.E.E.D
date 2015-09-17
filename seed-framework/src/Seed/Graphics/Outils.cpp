#include <Seed/Graphics/Outils.hpp>

void AfficherMatrices(glm::mat4 & mat)
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << mat[i].x << " " << mat[i].y << " " << mat[i].z << " " << mat[i].w << std::endl;
	}
}

void printErrorOpenGL()
{
	GLenum err;
	std::string error;
	while ((err = glGetError()) != GL_NO_ERROR) {
		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cerr << "OpenGL error: " << error << std::endl;
	}
}

void scanSeedError(unsigned int flag)
{
	switch (flag)
	{
	case 1:
		std::cout << "SUCCESS" << std::endl;
		break;
	case 50:
		std::cout << "Error, file not founding" << std::endl;
		break;
	case 51:
		std::cout << "Error, default shaders not founding" << std::endl;
		break;
	}
}