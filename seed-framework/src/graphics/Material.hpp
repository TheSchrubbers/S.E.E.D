/*!
* \file Material.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#ifdef _WIN32
	#include <GL\glew.h>
	#include <assimp\Importer.hpp>
	#include <assimp\scene.h>
#else
	#include <GL/glew.h>
	#include <assimp/Importer.hpp>
	#include <assimp/scene.h>
#endif

/*! \class Material
* \brief Material of a node
*/
class Material
{
public:
	/*!
	* \brief Constructor of class Material
	*/
	Material(const aiMaterial *material);
	~Material(){};
	/*!
	* \brief Load shaders from the directory "Shaders" in directory of this material
	*/
	void loadShaders();
	void render();
private:
	GLuint programID;
};

#endif