#version 440

#define MAX_NUM_TOTAL_LIGHTS 20
#define M_PI 3.1415926535897932384626433832795

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

in vec3 C;
in vec3 N;
in vec2 UV;
in vec3 P;
flat in uint sizeLights;

out vec4 Color;

struct Light
{
	float ambiant;
	float diffuse;
	float specular;
};
uniform Light light;
uniform sampler2D samplerAmbiantTexture0;

vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 color);
vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 color);
vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 color);

void main()
{
	vec3 texcolor = vec3(texture(samplerAmbiantTexture0, UV));
	vec3 PLContributing = vec3(0.0);
	//int sizeLights = pointLights[0].size.x + directionnalLights[0].size.x + spotLights[0].size.x + flashLights[0].size.x;
	for (int i = 0; i < pointLights[0].size.x; i++)
	{
		PLContributing += computeContributingPointLights(pointLights[i], N, C, P, texcolor);
	}
	for (int i = 0; i < directionnalLights[0].size.x; i++)
	{
		PLContributing += computeContributingDirectionnalLights(directionnalLights[i], N, C, texcolor);
	}
	for (int i = 0; i < spotLights[0].size.x; i++)
	{
		PLContributing += computeContributingSpotLights(spotLights[i], N, C, P, texcolor);
	}
	//Color = vec4(PLContributing, 1.0);
	//Color = vec4((1 / sizeLights) * computeContributingSpotLights(spotLights[0], N, C, P, texcolor) + (1 / sizeLights) * computeContributingDirectionnalLights(directionnalLights[0], N, C, texcolor), 1.0);
	Color = vec4(texcolor, 1.0);
}

vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 color)
{
	// Compute attenuation
	//compute the lenght of the vector
	float distance = length(pl.position.xyz - Po);
	float attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance +
		pl.attenuation.z * (distance * distance));
	vec3 L = normalize(pl.position.xyz - Po);
	vec3 Ia = attenuation * light.ambiant * color;
	vec3 Id = pl.color.xyz * attenuation * light.diffuse * color * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	vec3 Is = attenuation * light.specular * vec3(1.0) * pow(max(dot(R, Ca), 0), 10.0);

	return Ia + Id + Is;
}

vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 color)
{
	vec3 L = normalize(-pl.direction.xyz);
	vec3 Ia = light.ambiant * color;
	vec3 Id = light.diffuse * color * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	vec3 Is = light.specular * vec3(1.0) * pow(max(dot(R, Ca), 0), 10.0);

	return Ia + Id + Is;
}

vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 color)
{
	float a = acos(dot(normalize(pl.direction.xyz), normalize(Po - pl.position.xyz))) * (180.0 / M_PI);
	float attenuation = 0, distance = 0;

	distance = length(pl.position.xyz - Po);
	attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance +
		pl.attenuation.z * (distance * distance));
	vec3 Ia = attenuation * light.ambiant * color;
	if (a < pl.attenuation.w)
	{
		// Compute attenuation
		//compute the lenght of the vector
		vec3 L = normalize(pl.position.xyz - Po);
		vec3 Id = pl.color.xyz * attenuation * light.diffuse * color * max(dot(L, No), 0);
		vec3 R = normalize(reflect(L, No));
		vec3 Is = attenuation * light.specular * vec3(1.0) * pow(max(dot(R, Ca), 0), 10.0);
		return Ia + Id + Is;
	}
	return vec3(Ia);
}
/*Phong(vec3 P, vec3 L, vec3 V, vec3 color, float is, float id)
{
	vec3 vecL = normalize(L - P);
	return id * 
}*/