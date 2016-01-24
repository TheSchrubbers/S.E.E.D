/*
* Copyright (c) <2015> <Antoine Richard and Jérémy Riffet, S.E.E.D>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

/*!
* \file shader.hpp
* \brief Loader shaders
* \author Jérémy RIFFET
* \version 0.1
* \copyright Copyright (c) 2015,
* Antoine "Avzgui" Richard and Jérémy "Chtimy" Riffet, S.E.E.D
* \license Zlib License.
*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <GL/glew.h>
#include <vector>

class Shader
{
public:
	Shader(const std::string shader_dir_path, unsigned int *flag = nullptr);
	Shader();
	~Shader();
	bool useProgram();
	GLuint getID();
	bool loadUniqueShader(const std::string vertex_file_path);
	bool loadVertexFragmentShaders(const std::string vertex_file_path, const std::string fragment_file_path);
	void release();

private:
	GLuint programID;

	GLuint loadShaders(const std::string directory_file_path);
	GLuint loadCompileShader(std::string s, GLuint type, GLint &Result, int &InfoLogLength, std::vector<char> *ShaderErrorMessage);
	std::string parsingIncludes(std::string code);
	std::string replaceCodeByInclude(std::string code, std::vector<std::string> includes, std::vector<int> beg, std::vector<int> end);
};

#endif