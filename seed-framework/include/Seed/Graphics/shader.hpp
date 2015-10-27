#include <vector>
#include <fstream>
#include <iostream>
#include <string>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//class Shader
//{
//public:
	//Shader(const std::string shader_dir_path, const std::string name);
	//~Shader();
	GLuint loadShaders(const std::string vertex_file_path, const std::string fragment_file_path);
	GLuint loadShader(const std::string file_path);
	//void createNameARB(std::string name, std::string path);
//private:
	//std::string name;
	//GLuint programID;
//};

//void createShader(const std::string path, const std::string name);