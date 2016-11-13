#version 440

#define M_PI 3.1415926535897932384626433832795
#define NB_SHADOW_TEXTURES 2

#include "Lights.glsl"
#include "Camera.glsl"
#include "Phong-BlinnPhong.glsl"
#include "ShadowMap.glsl"

//OUT
out vec4 Color;

void main()
{
	//final color
	Color = vec4(1.0);
}