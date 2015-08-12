#include <vector>
#include <iostream>
#include <fstream>
#include "..\..\..\lib\glew-1.12.0\include\GL\glew.h"
#include "..\..\..\lib\glfw-3.1.1\include\GLFW\glfw3.h"
#include "..\..\..\lib\glm\glm\glm.hpp"
#include <string>

using namespace glm;


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);