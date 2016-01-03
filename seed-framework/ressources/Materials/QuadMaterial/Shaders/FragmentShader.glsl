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
	vec4 K;
};
struct SpotLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
	vec4 K;
};
struct DirectionnalLight
{
	vec4 direction;
	vec4 color;
	ivec4 size;
	vec4 K;
};
struct FlashLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
	vec4 K;
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

in vec2 UV;

flat in uint sizeLights;

//OUT
out vec4 Color;

//UNIFORM
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

//FUNCTIONS
/*vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec);
vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);
vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);*/

void main()
{
	vec3 Pos = texture(gPosition, UV).rgb;
	//process pointLights contributions
	/*for (int i = 0; i < pointLights[0].size.x; i++)
	{
		PLContributing += computeContributingPointLights(pointLights[i], Normal, C, P, difColor.xyz, specColor);
	}
	//process directionnalLights contributions
	for (int i = 0; i < directionnalLights[0].size.x; i++)
	{
		PLContributing += computeContributingDirectionnalLights(directionnalLights[i], Normal, C, difColor.xyz, specColor);
	}
	//process spotLights contributions
	for (int i = 0; i < spotLights[0].size.x; i++)
	{
		PLContributing += computeContributingSpotLights(spotLights[i], Normal, C, P, difColor.xyz, specColor);
	}
	//if the object has a positive reflective coefficient, it contributes
	if (mat.x > 0.0)
	{
		PLContributing = mat.x * skyboxColor.xyz + (1.0 - mat.x) * PLContributing;
	}*/
	Color = vec4(0.5);
	
	//Color = vec4(Pos, 1.0);
	//Color = vec4(0.5);
}

/*vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec)
{
	// Compute attenuation
	//compute the lenght of the vector
	float distance = length(pl.position.xyz - Po);
	//compute the attenuation of the light
	float attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance + pl.attenuation.z * (distance * distance));
	//vector light
	vec3 L = normalize(pl.position.xyz - Po);

	//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
	return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation);
}

vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec)
{
	vec3 L = normalize(-pl.direction.xyz);
	//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, 1.0);
	return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, 1.0);
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
		//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
		return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation);
	}
	Ia = attenuation * light.ambiant * diff;
	return vec3(Ia);
}*/

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
/*vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation)
{
	//compute ambiant contributing
	vec3 Ia = attenuation * Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(R, Ca), 0), 10.0);
	return Ia + Id + Is;
}*/

//Blinn	Phong : 
//vec3 color of the light -> LColor
//float coefficient ambiant -> Ka,
//float coefficient diffusion -> Kd,
//float coefficient specular -> Ks, 
//vec3 light -> L,
//vec3 Normal -> No,
//vec3 Camera -> Ca,
//vec3 diffMap -> diffMap,
//vec3 specMap -> specMap
/*vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation)
{
	vec3 H = normalize(L + Ca);
	//compute ambiant contributing
	vec3 Ia = attenuation * Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(N, H), 0), 10.0);
	return Ia + Id + Is;
}*/