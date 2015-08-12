#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

using namespace glm;


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);