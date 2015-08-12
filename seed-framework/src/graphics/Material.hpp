#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "..\..\..\lib\glew-1.12.0\include\GL\glew.h"

/*!
* \file Material.hpp
* \brief Material of a node
* \author Jérémy RIFFET
* \version 0.1
*/

/*! \class Material
* \brief Material of a node
*/
class Material
{
public:
	/*!
	* \brief Constructor of class Material
	*/
	Material();
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