#include <Seed/Graphics/shader.hpp>
#include <boost/filesystem.hpp>
#include <Seed/Graphics/Constant.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#define NBSHADERS 3


Shader::Shader(const std::string shader_dir_path, unsigned int *flag)
{
	this->programID = loadShaders(shader_dir_path);
	if (flag)
	{
		if (!this->programID)
			*flag = SEED_ERROR_LOAD_SHADER;
		else
			*flag = SEED_SUCCESS;
	}
}

Shader::~Shader()
{
	glDeleteProgram(this->programID);
}


GLuint Shader::loadShaders(const std::string directory_file_path)
{
	//open path
	boost::filesystem::path p(directory_file_path);
	std::string n,s;
	GLuint shaders[NBSHADERS] = { 0, 0, 0 };

	GLint Result = GL_FALSE;
	int InfoLogLength;
	std::vector<char> *ShaderErrorMessage = new std::vector<char>;

	//if the path exists
	if (boost::filesystem::exists(p))
	{
		//if is it a directory
		if (boost::filesystem::is_directory(p))
		{
			//get the file containing in the directory
			std::vector<boost::filesystem::path> paths;
			std::copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), std::back_inserter(paths));
			sort(paths.begin(), paths.end());

			//check all the filenames
			for (std::vector<boost::filesystem::path>::const_iterator it(paths.begin()); it != paths.end(); it++)
			{
				n = it->filename().stem().string();
				s = it->relative_path().string();
				if (n == "GeometryShader")
				{
					shaders[0] = this->loadCompileShader(s, GL_GEOMETRY_SHADER, Result, InfoLogLength, ShaderErrorMessage);
				}
				else if (n == "VertexShader")
				{
					shaders[1] = this->loadCompileShader(s, GL_VERTEX_SHADER, Result, InfoLogLength, ShaderErrorMessage);
				}
				else if (n == "FragmentShader")
				{
					shaders[2] = this->loadCompileShader(s, GL_FRAGMENT_SHADER, Result, InfoLogLength, ShaderErrorMessage);
				}
			}

			// Link the program
			fprintf(stdout, "Linking program\n");
			GLuint ProgramID = glCreateProgram();
			for (int i = 0; i < NBSHADERS; i++)
			{
				if (shaders[i])
				{
					glAttachShader(ProgramID, shaders[i]);
				}
			}
			glLinkProgram(ProgramID);
			// Check the program
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			//fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

			for (int i = 0; i < NBSHADERS; i++)
				if (shaders[i])
					glDeleteShader(shaders[i]);
			return ProgramID;
		}
	}
	std::cout << "Error, path of shaders' directory unfound, " << directory_file_path << std::endl;;
	return 0;
}

GLuint Shader::loadCompileShader(std::string s, GLuint type, GLint &Result, int &InfoLogLength, std::vector<char> *ShaderErrorMessage)
{
	// Create the shaders
	GLuint ShaderID = glCreateShader(type);
	// Read the Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(s, std::ios::in);
	std::string Line = "";

	if (ShaderStream.is_open())
	{
		while (getline(ShaderStream, Line))
		{
			ShaderCode += "\n" + Line;
		}
		ShaderStream.close();

		printf("Compiling shader : %s\n", s);
		
		char const * SourcePointer = ShaderCode.c_str();

		//compile shader
		glShaderSource(ShaderID, 1, &SourcePointer, NULL);
		glCompileShader(ShaderID);

		// Check Vertex Shader
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> SEM(InfoLogLength);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &SEM[0]);
		if (&SEM[0])
			fprintf(stdout, "%s\n", &SEM[0]);
		ShaderErrorMessage = &SEM;
		return ShaderID;
	}
	else
	{
		std::cout << "ERROR: error the vertexshader file \"" << s << "\" is not found." << std::endl;
		return 0;
	}
}

GLuint Shader::getID()
{
	return this->programID;
}

void Shader::useProgram()
{
	glUseProgram(this->programID);
}

GLuint Shader::loadShaders(const std::string vertex_file_path, const std::string fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else
	{
		std::cout << "ERROR: error the vertexshader file \"" << vertex_file_path << "\" is not found." << std::endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path.c_str(), std::ios::in);

	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else
	{
		std::cout << "ERROR: error the fragment shader file \"" << fragment_file_path << "\" is not found." << std::endl;
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
