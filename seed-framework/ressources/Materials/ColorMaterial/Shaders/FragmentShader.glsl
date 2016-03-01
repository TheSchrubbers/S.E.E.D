#version 440

#define M_PI 3.1415926535897932384626433832795
#define NB_SHADOW_TEXTURES 2

#include "Lights.glsl"
#include "Camera.glsl"
#include "Phong-BlinnPhong.glsl"
#include "ShadowMap.glsl"

struct Light
{
	float ambiant;
	float diffuse;
	float specular;
};

in vec3 C;
in vec3 P;
in vec3 N;
in vec4 PInLightSpace[NB_SHADOW_TEXTURES];
in vec3 LShadow;
flat in uint sizeLights;

//OUT
out vec4 Color;

//UNIFORM
uniform Light light;
uniform samplerCube skybox;
uniform sampler2D gShadowMap;
uniform sampler2D gShadowMap2;
uniform vec2 mat;
uniform vec3 color;
uniform bool shadowsActive;
uniform float bias;

//FUNCTIONS
vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec, float shadow);
vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec, float shadow);
vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec, float shadow);

void main()
{
	//Process Shadows
	float shadow = 0.0;
	if(shadowsActive)
	{
		shadow += processPerspectiveShadow(PInLightSpace[0], gShadowMap, bias);
		shadow += processPerspectiveShadow(PInLightSpace[1], gShadowMap2, bias);
		shadow /= 2.0f;
	}
	vec3 Reflect;
	vec4 skyboxColor;
	if (mat.x > 0.0)
	{
		//reflection color
		Reflect = reflect(C, N);
		skyboxColor = texture(skybox, Reflect);
	}
	//refraction Color
	vec4 difColor = vec4(color, 1.0);
	vec3 specColor = vec3(1.0);
	
	vec3 PLContributing = vec3(0.0);

	vec3 Normal = N;

	//process pointLights contributions
	for (int i = 0; i < pointLights[0].size.x; i++)
	{
		PLContributing += computeContributingPointLights(pointLights[i], Normal, C, P, difColor.xyz, specColor, shadow);
	}
	//process directionnalLights contributions
	for (int i = 0; i < directionnalLights[0].size.x; i++)
	{
		PLContributing += computeContributingDirectionnalLights(directionnalLights[i], Normal, C, difColor.xyz, specColor, shadow);
	}
	//process spotLights contributions
	for (int i = 0; i < spotLights[0].size.x; i++)
	{
		PLContributing += computeContributingSpotLights(spotLights[i], Normal, C, P, difColor.xyz, specColor, shadow);
	}
	//if the object has a positive reflective coefficient, it contributes
	/*if (mat.x > 0.0)
	{
		PLContributing = mat.x * skyboxColor.xyz + (1.0 - mat.x) * PLContributing;
	}*/

	//final color
	Color = vec4(PLContributing, 1.0);
	//Color = vec4(texture(gShadowMap, P.xy).xyz, 1.0);
}

vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec, float shadow)
{
	// Compute attenuation
	//compute the lenght of the vector
	float distance = length(pl.position.xyz - Po);
	//compute the attenuation of the light
	//float attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance + pl.attenuation.z * (distance * distance));
	float attenuation = 1.0f / (1.0f + distance * distance * pl.attenuation.x);
	//vector light
	vec3 L = normalize(pl.position.xyz - Po);

	return Phong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation, shadow);
}

vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec, float shadow)
{
	vec3 L = normalize(-pl.direction.xyz);
	return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, 1.0);
}

vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec, float shadow)
{
	float a = acos(dot(normalize(pl.direction.xyz), normalize(Po - pl.position.xyz))) * (180.0 / M_PI);
	float attenuation = 0, distance = 0;
	vec3 Ia;
	distance = length(pl.position.xyz - Po);
	//attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance + pl.attenuation.z * (distance * distance));
	attenuation = 1.0f / (1.0f + distance * distance* pl.attenuation.x);
	if (a < pl.attenuation.w)
	{
		// Compute attenuation
		//compute the lenght of the vector
		vec3 L = normalize(pl.position.xyz - Po);
		return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation);
	}
	Ia = light.ambiant * diff;
	return vec3(Ia);
}