#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <iostream>
#include <map>

const int WIDTH = 1024;
const int HEIGHT = 768;

const std::string pathToShaders = "ressources/Materials/DefaultMaterial/Shaders/";
const std::string pathToTextures = "ressources/textures/";
const std::string pathToModels = "ressources/models/";
const std::string pathToDefaultShadersDir = "default/";

const std::map<int, int> distToAttenuation = { { 7, 0 }, { 13, 1 }, { 20, 2 }, { 32, 3 }, { 50, 4 }, { 65, 5 }, { 100, 6 }, { 160, 7 }, { 200, 8 }, { 325, 9 }, { 600, 10 }, { 3250, 11 } };
const int Attenuation[3][12] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.7, 0.35, 0.22, 0.14, 0.09, 0.07, 0.045, 0.027, 0.022, 0.014, 0.007, 0.0014, 1.8, 0.44, 0.20, 0.07, 0.032, 0.017, 0.0075, 0.0028, 0.0019, 0.0007, 0.0002, 0.000007 };

#define TEXTURE_AMBIANT 10
#define TEXTURE_DIFFUSE 11
#define TEXTURE_SPECULAR 12

#define BMP_FORMAT 20
#define PNG_FORMAT 21
#define JPEG_FORMAT 22
#define TGA_FORMAT 23

#define SEED_ERROR_FILE_LOCATION 50;
#define SEED_ERROR_DEFAULT_SHADER_NOT_FOUND 51
#define SEED_SUCCESS 1

#endif