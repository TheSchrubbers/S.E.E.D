#include <vector>
#include <fstream>
#include <iostream>


#ifdef _WIN32
	#include <GL/glew.h>
	#include <GLFW\glfw3.h>
	#include <glm\glm.hpp>
#else
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
	#include <glm/glm.hpp>
#endif

GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);
