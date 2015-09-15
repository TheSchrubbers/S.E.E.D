#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <iostream>

const int WIDTH = 1024;
const int HEIGHT = 768;

#ifdef _WIN32
	const std::string pathToShaders = "ressources\\Shaders\\";
	const std::string pathToTextures = "ressources\\textures\\";
	const std::string pathToModels = "ressources\\models\\";
#else	
	const std::string pathToShaders = "ressources/Shaders/";
	const std::string pathToTextures = "ressources/textures/";
	const std::string pathToModels = "ressources/models/";
#endif


#endif