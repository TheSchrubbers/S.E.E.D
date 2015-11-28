#version 440

#define MAX_NUM_TOTAL_LIGHTS 20
#define M_PI 3.1415926535897932384626433832795

//Structs
struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
};
struct SpotLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
};
struct DirectionnalLight
{
	vec4 direction;
	vec4 color;
	ivec4 size;
};
struct FlashLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
};

struct Light
{
	float ambiant;
	float diffuse;
	float specular;
};

//IN
layout(std140, binding = 0) uniform PointLightsBuffer
{
	PointLight pointLights[MAX_NUM_TOTAL_LIGHTS];
};
layout(std140, binding = 1) uniform SpotLightsBuffer
{
	SpotLight spotLights[MAX_NUM_TOTAL_LIGHTS];
};
layout(std140, binding = 2) uniform DirectionnalLightsBuffer
{
	DirectionnalLight directionnalLights[MAX_NUM_TOTAL_LIGHTS];
};
layout(std140, binding = 3) uniform FlashLightsBuffer
{
	FlashLight flashLights[MAX_NUM_TOTAL_LIGHTS];
};

flat in uint sizeLights;
in vec3 C;
in vec3 P;
in vec3 N;

//OUT
out vec4 Color;

//UNIFORM
uniform Light light;
uniform sampler2D samplerAmbiantTexture0;
uniform sampler2D samplerSpecularTexture0;
uniform sampler2D samplerNormalTexture;
uniform samplerCube skybox;
uniform vec2 mat;
uniform bool NormalMappingActive, SpecularMappingActive, SpecularMappingView;

//FUNCTIONS
vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec);
vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec3 Phong(vec3 LColor, float Ka, float Kd, float Ks, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);

void main()
{
	vec3 Reflect;
	vec4 skyboxColor;
	if (mat.x > 0.0)
	{
		//reflection color
		Reflect = reflect(C, N);
		skyboxColor = texture(skybox, Reflect);
	}

	vec3 PLContributing = vec3(0.0);

	vec3 Normal = N;

	//process pointLights contributions
	for (int i = 0; i < pointLights[0].size.x; i++)
	{
		PLContributing += computeContributingPointLights(pointLights[i], Normal, C, P, vec3(1.0), vec3(1.0));
	}
	//process directionnalLights contributions
	for (int i = 0; i < directionnalLights[0].size.x; i++)
	{
		PLContributing += computeContributingDirectionnalLights(directionnalLights[i], Normal, C, vec3(1.0), vec3(1.0));
	}
	//process spotLights contributions
	for (int i = 0; i < spotLights[0].size.x; i++)
	{
		PLContributing += computeContributingSpotLights(spotLights[i], Normal, C, P, vec3(1.0), vec3(1.0));
	}
	//if the object has a positive reflective coefficient, it contributes
	if (mat.x > 0.0)
	{
		PLContributing = mat.x * skyboxColor.xyz + (1.0 - mat.x) * PLContributing;
	}

	//final color
	Color = vec4(PLContributing, 1.0);
	//Color = vec4(1.0);
}

vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec)
{
	// Compute attenuation
	//compute the lenght of the vector
	float distance = length(pl.position.xyz - Po);
	//compute the attenuation of the light
	float attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance +
		pl.attenuation.z * (distance * distance));
	//vector light
	vec3 L = normalize(pl.position.xyz - Po);

	return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
}

vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec)
{
	vec3 L = normalize(-pl.direction.xyz);
	return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, 1.0);
}

vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec)
{
	float a = acos(dot(normalize(pl.direction.xyz), normalize(Po - pl.position.xyz))) * (180.0 / M_PI);
	float attenuation = 0, distance = 0;
	vec3 Ia;
	distance = length(pl.position.xyz - Po);
	attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance + pl.attenuation.z * (distance * distance));
	if (a < pl.attenuation.w)
	{
		// Compute attenuation
		//compute the lenght of the vector
		vec3 L = normalize(pl.position.xyz - Po);
		return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
	}
	Ia = attenuation * light.ambiant * diff;
	return vec3(Ia);
}

//Phong : 
//vec3 color of the light -> LColor
//float coefficient ambiant -> Ka,
//float coefficient diffusion -> Kd,
//float coefficient specular -> Ks, 
//vec3 light -> L,
//vec3 Normal -> No,
//vec3 Camera -> Ca,
//vec3 diffMap -> diffMap,
//vec3 specMap -> specMap
vec3 Phong(vec3 LColor, float Ka, float Kd, float Ks, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation)
{
	//compute ambiant contributing
	vec3 Ia = attenuation * Ka * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Kd * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Ks * specMap * pow(max(dot(R, Ca), 0), 10.0);
	return Ia + Id + Is;
}