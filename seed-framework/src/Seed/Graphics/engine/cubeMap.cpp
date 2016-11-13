//SEED INCLUDES
#include "Seed/Graphics/engine/cubeMap.hpp"
#include "Seed/Graphics/engine/texture.hpp"
#include "Seed/Graphics/model/geometry.hpp"
#include "Seed/Graphics/engine/shader.hpp"
#include "Seed/Graphics/loaders/parserImage.hpp"
#include "Seed/Graphics/engine/scene.hpp"
#include "Seed/Graphics/buffers/UBOBuffer.hpp"
#include "Seed/Graphics/engine/camera.hpp"
//OTHER INCLUDES
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

using boost::filesystem::directory_iterator;
using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_directory;
using boost::filesystem::directory_iterator ;
using std::vector;

CubeMap::CubeMap(const std::string path, std::shared_ptr<Scene> sce, unsigned int *flag)
{
	std::cout << "initialize GMap" << std::endl;
    initializeOpenGLFunctions();

	//init variable
	this->VAO = 0;
	this->block_index_camera = 0;
	this->shader = nullptr;
	this->textureID = 0;
	this->VBOVertices = 0;

	this->createCube();
	this->scene = sce;
	if (!flag)
	{
		flag = new unsigned int;
	}
	std::cout << this->loadTextures(path) << std::endl;
	if (!this->loadTextures(path))
	{
		std::cout << "flag" << std::endl;
		*flag = SEED_ERROR_FILE_LOCATION;
	}
	std::cout << "flag" << std::endl;
	if (!this->createShader())
	{
		*flag = SEED_ERROR_DEFAULT_SHADER_NOT_FOUND;
	}
	std::cout << "fin initialisation GMap" << std::endl;

}

CubeMap::~CubeMap()
{
	delete this-> shader;
}

bool CubeMap::loadTextures(const std::string pathDirectory)
{
	std::cout << "loadTextures" << std::endl;
	int width, height, t;
	parserImage *img = new parserImage();
	//we want all the filenames in the directory
	path p(pathDirectory);
	bool done[6] = { false };

	std::string s, n;
	//create a new openGL texture
	glGenTextures(1, &textureID);
	//bind texture to modify this -> typetexture2D
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);	
	std::cout << p << std::endl;
	std::cout << exists(p) << std::endl;
	std::cout << is_directory(p) << std::endl;
	std::cout << is_regular_file(p) << std::endl;

	if (exists(p) && is_directory(p))
	{
		vector<path> paths;
		std::cout << "test" << std::endl;
		printValue("path", p.relative_path().string());
		std::copy(directory_iterator(p), directory_iterator(), std::back_inserter(paths));
		sort(paths.begin(), paths.end());
		//check all the filenames
		for (vector<path>::const_iterator it(paths.begin()); it != paths.end(); it++)
		{
			s = it->relative_path().string();
			n = it->filename().stem().string();
			if (n == "back" && !done[1])
			{
				this->loadFace(1, &s, img);
				done[1] = true;
			}
			else if (n == "bottom" && !done[4])
			{
				this->loadFace(4, &s, img);
				done[4] = true;
			}
			else if (n == "front" && !done[0])
			{
				this->loadFace(0, &s, img);
				done[0] = true;
			}
			else if (n == "left" && !done[3])
			{
				this->loadFace(3, &s, img);
				done[3] = true;
			}
			else if (n == "right" && !done[2])
			{
				this->loadFace(2, &s, img);
				done[2] = true;
			}
			else if (n == "top" && !done[5])
			{
				this->loadFace(5, &s, img);
				done[5] = true;
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		std::cout << "fin loadTextures" << std::endl;


		return true;
	}
	else
	{
		std::cout << "writelog" << std::endl;
		writeLog("ERROR -> CubeMap : File " + pathDirectory + " not found.");
		std::cout << "fin writelog" << std::endl;

	}
	return false;

}

bool CubeMap::loadFace(int type, const std::string *path, parserImage *img)
{
	//load image
	if (img->readImage(*path))
	{
		switch (type)
		{
			case 0:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
			case 1:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
			case 2:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
			case 3:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
			case 4:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
			case 5:
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, img->getDepth(), img->getWidth(), img->getHeight(), 0, img->getDepth(), GL_UNSIGNED_BYTE, img->getPixels());
				break;
		}
		return true;
	}
	else
	{
		writeLog("ERROR -> CubeMap: Error opening file " + *path);
		return false;
	}
}

void CubeMap::createCube()
{
	GLfloat cubeVertices[] = {
         
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	//VAO generation
	glGenVertexArrays(1, &(this->VAO));
	glGenBuffers(1, &VBOVertices);
	//bind VAO
	glBindVertexArray(this->VAO);
	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	//allocate memory for the faces
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), 0, GL_STATIC_DRAW);
	//put the faces in the buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//free VAO
	glBindVertexArray(0);
}

void CubeMap::draw()
{
	if (this->shader)
	{
		//load shaders in memory
		this->shader->useProgram();
		glDepthFunc(GL_LEQUAL);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->scene->getCamera()->getUBOId());
		//bind UBO camera with program shader
		glUniformBlockBinding(this->shader->getID(), this->block_index_camera, 0);
		glActiveTexture(GL_TEXTURE0);
		this->bind();
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		this->release();
		glDepthMask(GL_TRUE);
	}
}

bool CubeMap::createShader()
{
	unsigned int flag;
	//load shaders
	this->shader = new Shader(pathToMaterials + "CubeMapMaterial/Shaders", &flag);
	//if shaders not loading return false
	if (flag != SEED_SUCCESS)
	{
		return false;
	}
	//bind UBO camera
	this->block_index_camera = glGetUniformBlockIndex(this->shader->getID(), "CameraBuffer");
	return true;
}

void CubeMap::bind()
{
	if (this->textureID != 0)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
	}
}
void CubeMap::release()
{
	if (this->textureID != 0)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}