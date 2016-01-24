#version 440

#define MAX_NUM_TOTAL_LIGHTS 3
#define M_PI 3.1415926535897932384626433832795
#define NB_SHADOW_TEXTURES 2

//Structs
struct PointLight
{
	vec4 position;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
	vec4 K;
	mat4 VP;
};
struct SpotLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
	vec4 K;
	mat4 VP;
};
struct DirectionnalLight
{
	vec4 direction;
	vec4 color;
	ivec4 size;
	vec4 K;
	mat4 VP;
};
struct FlashLight
{
	vec4 position;
	vec4 direction;
	vec4 color;
	vec4 attenuation;
	ivec4 size;
	vec4 K;
	mat4 VP;
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
vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation, float shadow);
vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);
float processPerspectiveShadow(vec4 lightInSpace, sampler2D shadowTexture);
float processOrthographicShadow(vec4 lightInSpace, sampler2D shadowTexture);
float LinearizeDepth(float depth);

void main()
{
	//Process Shadows
	float shadow = 0.0;
	if(shadowsActive)
	{
		shadow += processPerspectiveShadow(PInLightSpace[0], gShadowMap);
		shadow += processPerspectiveShadow(PInLightSpace[1], gShadowMap2);
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
	//Color =vec4(vec3(shadow), 1.0);
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
vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation, float shadow)
{
	//compute ambiant contributing
	vec3 Ia = Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(R, Ca), 0), 10.0);
	return Ia + (1.0 - shadow) * (Id + Is);
}

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
vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation)
{
	vec3 H = normalize(L + Ca);
	//compute ambiant contributing
	vec3 Ia = attenuation * Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(No,H), 0.0), 10.0);
	return Ia + Id + Is;
}

float processPerspectiveShadow(vec4 lightInSpace, sampler2D  gShadow)
{
	float shadowDepth;
	vec3 l = lightInSpace.xyz / lightInSpace.w;
	l = l * 0.5 + 0.5;
	l.z = LinearizeDepth(l.z) / 30.0;

	float shadow = 0.0;

	if(l.z <= 1.0)
	{
		//size of a texel on the gshadowmaptexture
		vec2 texelSize = 1.0 / textureSize(gShadow, 0);
		//use BLUR to smooth shadow
		for(int i = -1; i <= 1; ++i)
		{
			for(int j = -1; j <= 1; ++j)
			{
				//corresponding depth point in shadowmap pass
				shadowDepth = texture(gShadow, l.xy + vec2(i, j) * texelSize).x;
				shadowDepth = LinearizeDepth(shadowDepth) / 30.0;
				if(shadowDepth < (l.z - bias))
				{
					shadow += 1.0;
				}		
			}
		}
		shadow /= 9.0;
		//bias between 0.005 and 0.05
		//float bias = max(0.05 * (1.0 - dot(N, LShadow)), 0.005);
		
	}
	return shadow;
}

float processOrthographicShadow(vec4 lightInSpace)
{
	float shadowDepth;
	vec3 l = lightInSpace.xyz / lightInSpace.w;
	l = l * 0.5 + 0.5;

	float shadow = 0.0;

	if(l.z <= 1.0)
	{
		//size of a texel on the gshadowmaptexture
		vec2 texelSize = 1.0 / textureSize(gShadowMap, 0);
		//use BLUR to smooth shadow
		for(int i = -1; i <= 1; ++i)
		{
			for(int j = -1; j <= 1; ++j)
			{
				//corresponding depth point in shadowmap pass
				shadowDepth = texture(gShadowMap, l.xy + vec2(i, j) * texelSize).x;
				if(shadowDepth < (l.z - bias))
				{
					shadow += 1.0;
				}		
			}
		}
		shadow /= 9.0;
		//bias between 0.005 and 0.05
		//float bias = max(0.05 * (1.0 - dot(N, LShadow)), 0.005);
		
	}
	return shadow;
}


float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * 1.0 * 30.0) / (30.0 + 1.0 - z * (30.0 - 1.0));
}
