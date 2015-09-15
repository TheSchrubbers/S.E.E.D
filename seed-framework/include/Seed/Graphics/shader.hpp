#include <vector>
#include <fstream>
#include <iostream>
#include <string>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLuint loadShaders(const std::string vertex_file_path, const std::string fragment_file_path);