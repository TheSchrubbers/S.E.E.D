#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <iostream>
#include <map>

const int WIDTH = 1024;
const int HEIGHT = 768;

const std::string pathToDefaultMaterial = "ressources/Materials/DefaultMaterial/";
const std::string pathToImplicitMaterial = "ressources/Materials/ImplicitMaterial/";
const std::string pathToTextures = "ressources/Textures/";
const std::string pathToCubeMapMaterial = "ressources/Materials/CubeMapMaterial/";
const std::string pathToParticlesWaterSystemMaterial = "ressources/Materials/ParticlesSystemMaterial/ParticlesWaterSystemMaterial/";
const std::string pathToSPHMaterial = "ressources/Materials/ParticlesSystemMaterial/SPHMaterial/";
const std::string pathToModels = "ressources/Models/";
const std::string pathToBasicModels = "ressources/Models/BasicMesh/";
const std::string pathToMaterials = "ressources/Materials/";


const std::map<int, int> distToAttenuation = { { 7, 0 }, { 13, 1 }, { 20, 2 }, { 32, 3 }, { 50, 4 }, { 65, 5 }, { 100, 6 }, { 160, 7 }, { 200, 8 }, { 325, 9 }, { 600, 10 }, { 3250, 11 } };
const float Attenuation[3][12] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.7, 0.35, 0.22, 0.14, 0.09, 0.07, 0.045, 0.027, 0.022, 0.014, 0.007, 0.0014, 1.8, 0.44, 0.20, 0.07, 0.032, 0.017, 0.0075, 0.0028, 0.0019, 0.0007, 0.0002, 0.000007 };

#define SEED_TEXTURE_AMBIANT 10
#define SEED_TEXTURE_DIFFUSE 11
#define SEED_TEXTURE_SPECULAR 12
#define SEED_TEXTURE_NORMAL 13
#define SEED_CUBEMAP 18

#define SEED_BMP_FORMAT 20
#define SEED_PNG_FORMAT 21
#define SEED_JPEG_FORMAT 22
#define SEED_TGA_FORMAT 23

#define SEED_CIRCLE 80
#define SEED_POINT 81
#define SEED_SPHERE 82
#define SEED_SQUARE 83


#define SEED_ERROR_FILE_LOCATION 50
#define SEED_ERROR_DEFAULT_SHADER_NOT_FOUND 51
#define SEED_ERROR_LOAD_SHADER 52
#define SEED_SUCCESS 1

#define SEED_PI 3.14159265359

#define uniform GLuint

#endif