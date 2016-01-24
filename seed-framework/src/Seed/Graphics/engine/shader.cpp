//SEED INCLUDES
#include <Seed/Graphics/engine/shader.hpp>
#include <Seed/Graphics/buffers/UBOBuffer.hpp>
#include <Seed/Graphics/engine/Constant.hpp>
//OTHER INCLUDES
#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

//DEFINES
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

Shader::Shader()
{
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
			//fprintf(stdout, "Linking program\n");
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
	writeLog("			Error -> Path of shaders : directory unfound, " + directory_file_path);
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
		//Parsing code includes
		ShaderCode = this->parsingIncludes(ShaderCode);
		ShaderStream.close();

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
		{
			writeLog("Shader : " + s);
			writeLog(&SEM[0]);
		}
		ShaderErrorMessage = &SEM;
		return ShaderID;
	}
	else
	{
		writeLog("			ERROR -> Vertexshader file \"" + s + "\", not found.");
		return 0;
	}
}

GLuint Shader::getID()
{
	return this->programID;
}

bool Shader::useProgram()
{
	if (this->programID)
	{
		glUseProgram(this->programID);
		return true;
	}
	return false;
}

void Shader::release()
{
	glUseProgram(0);
}

bool Shader::loadVertexFragmentShaders(const std::string vertex_file_path, const std::string fragment_file_path)
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
		//Parsing code includes
		VertexShaderCode = this->parsingIncludes(VertexShaderCode);
	}
	else
	{
		writeLog("			ERROR -> Vertexshader file \"" + vertex_file_path + "\", not found.");
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
		//Parsing code includes
		FragmentShaderCode = this->parsingIncludes(FragmentShaderCode);
	}
	else
	{
		writeLog("			ERROR -> Fragment shader file \"" + fragment_file_path + "\", not found.");
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path.c_str());
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

	this->programID = ProgramID;

	return true;
}

bool Shader::loadUniqueShader(const std::string vertex_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);

	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
		//Parsing code includes
		VertexShaderCode = this->parsingIncludes(VertexShaderCode);
	}
	else
	{
		std::cout << "			ERROR -> Vertexshader file \"" << vertex_file_path << "\", not found." << std::endl;
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

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);

	this->programID = ProgramID;

	return true;
}


std::string Shader::parsingIncludes(std::string code)
{
	int size = code.size(), b, e;
	std::vector<std::string> includes;
	std::vector<int> beg;
	std::vector<int> end;
	//check all the code
	for (int i = 0; i < size; i++)
	{
		//if the following characters are -> #include
		if(code[i] == '#' && i+7 < size && code[i+1] == 'i' && code[i+2] == 'n' && code[i+3] == 'c' && code[i+4] == 'l' && code[i+5] == 'u' && code[i+6] == 'd' && code[i+7] == 'e')
		{
			b = i;
			int j = i + 8;
			//avoid spaces
			while(j < size && code[j] != '\"' && code[j] == ' ')
			{
				j++;
			}
			j++;
			std::string tmp;
			//check the name on inlude
			while(j < size && code[j] != '\"')
			{
				tmp.push_back(code[j]);
				j++;
			}
			e = j+1;
			//add the name to the list of includes
			includes.push_back(tmp);
			beg.push_back(b);
			end.push_back(e);
			i = j+1;
		}
	}

	code = replaceCodeByInclude(code, includes, beg, end);

	return code;
}

std::string Shader::replaceCodeByInclude(std::string code, std::vector<std::string> includes, std::vector<int> beg, std::vector<int> end)
{
	int b = 0;
	int e = 0;
	std::string out;
	std::string part = "";
	std::string line = "";
	int size = includes.size();
	if(size > 0)
	{
		//begin of the code
		for(int j = 0; j < beg[0]; j++)
		{
			out += code[j];
		}
		//first include
		std::ifstream file(pathToMaterials + "Common/" + includes[0], std::ios::in);
		if (file.is_open())
		{
			while (getline(file, line))
				part += "\n" + line;
			file.close();
			out += part;
			part.clear();
		}
		else
		{
			writeLog("			ERROR -> Path to a common code shader doesn't exist : " + includes[0]);
			return code;
		}
		//other includes
		for(int i = 1; i < includes.size(); i++)
		{
			//part of code
			for(int j = end[i-1]; j < beg[i]; j++)
			{
				out += code[j];
			}
			//include code
			std::ifstream file(pathToMaterials + "Common/" + includes[i], std::ios::in);
			if (file.is_open())
			{
				while (getline(file, line))
					part += "\n" + line;
				file.close();
				out += part;
				part.clear();
			}
			else
			{
				writeLog("			ERROR -> Path to a common code shader doesn't exist : " + includes[i]);
				return code;
			}
		}
		//end of code
		for(int j = end[size-1]; j < code.size(); j++)
		{
			out += code[j];
		}
		return out;
	}
	return code;
	
}
