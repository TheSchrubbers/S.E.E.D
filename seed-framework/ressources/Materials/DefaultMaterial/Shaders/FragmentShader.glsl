#version 440

#define M_PI 3.1415926535897932384626433832795

#include "Lights.glsl"
#include "Phong-BlinnPhong.glsl"

struct Light
{
	float ambiant;
	float diffuse;
	float specular;
};

in VERTEX_OUT
{
	vec3 PinWorldSpace;
	vec3 NinWorldSpace;
	vec3 VdirWorldSpace;

	vec3 PinFragSpace;
	vec3 VdirFragSpace;

	vec2 UV;

	mat3 TBN;
	mat3 transposeTBN;
}V_IN;

//OUT
out vec4 Color;

//UNIFORM
uniform Light light;
uniform sampler2D samplerAmbiantTexture0;
uniform sampler2D samplerSpecularTexture0;
uniform sampler2D samplerNormalTexture;
uniform sampler2D samplerDepthMapTexture;
uniform samplerCube skybox;
uniform vec2 mat;
uniform bool NormalMapActive, NormalMapView, SpecularMapActive, SpecularMapView, ParallaxMapActive, ParallaxMapView;
uniform float biasParallax;

//FUNCTIONS
vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec);
vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec);
vec2 ParallaxMapping(sampler2D heightMap, vec2 UVcoords, vec3 Vdir, float bParallax);

void main()
{
	vec3 Reflect = vec3(0.0);
	vec4 skyboxColor = vec4(0.0);
	vec3 Normal = vec3(0.0);
	vec2 UVcoords = vec2(0.0);
	vec3 difColor = vec3(0.0);
	vec3 specColor = vec3(1.0);
	vec3 PLContributing = vec3(0.0);

	if(ParallaxMapView)
		Color = vec4(vec3(texture(samplerDepthMapTexture, V_IN.UV).r), 1.0);
	else if(SpecularMapView)
		Color = vec4(texture(samplerSpecularTexture0, V_IN.UV).xyz, 1.0);
	else if(NormalMapView)
		Color = vec4(texture(samplerNormalTexture, V_IN.UV).xyz, 1.0);
	else
	{

		/*if (mat.x > 0.0)
		{
			//reflection color
			Reflect = reflect(C, N);
			skyboxColor = texture(skybox, Reflect);
		}*/

		if (ParallaxMapActive)
		{
			//height map
			UVcoords = ParallaxMapping(samplerDepthMapTexture, V_IN.UV, V_IN.VdirFragSpace, biasParallax);
		}
		else
		{
			UVcoords = V_IN.UV;
		}

		//diffuse map
		difColor = texture(samplerAmbiantTexture0, UVcoords).rgb;

		if (SpecularMapActive)
		{
			//specular map
			specColor = texture(samplerSpecularTexture0, UVcoords).rgb;
		}
		else
		{
			specColor = vec3(1.0);
		}
		
		if (NormalMapActive)
		{
			//normal map
			//clamping normal from [0,1] to [-1,1]
			Normal = normalize(texture(samplerNormalTexture, UVcoords).rgb * 2.0 - 1.0);
		}
		else
		{
			Normal = V_IN.NinWorldSpace;
		}

		//process pointLights contributions
		for (int i = 0; i < pointLights[0].size.x; i++)
		{
			PLContributing += computeContributingPointLights(pointLights[i], Normal, V_IN.VdirFragSpace, V_IN.PinFragSpace, difColor.xyz, specColor);
		}
		//process directionnalLights contributions
		/*for (int i = 0; i < directionnalLights[0].size.x; i++)
		{
			PLContributing += computeContributingDirectionnalLights(directionnalLights[i], Normal, C, difColor.xyz, specColor);
		}
		//process spotLights contributions
		for (int i = 0; i < spotLights[0].size.x; i++)
		{
			PLContributing += computeContributingSpotLights(spotLights[i], Normal, C, P, difColor.xyz, specColor);
		}*/
		//if the object has a positive reflective coefficient, it contributes
		if (mat.x > 0.0)
		{
			PLContributing = mat.x * skyboxColor.xyz + (1.0 - mat.x) * PLContributing;
		}

		Color = vec4(PLContributing, 1.0);
	}
}

vec3 computeContributingPointLights(PointLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec)
{
	//compute the lenght of the vector
	float distance = length(V_IN.transposeTBN * pl.position.xyz - Po);
	//compute the attenuation of the light
	float attenuation = 1.0f / (1.0f + distance * distance * pl.attenuation.x);
	//vector light
	vec3 L = normalize(V_IN.transposeTBN * pl.position.xyz - Po);

	//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
	return Phong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation, 0.0);
}

vec3 computeContributingDirectionnalLights(DirectionnalLight pl, vec3 No, vec3 Ca, vec3 diff, vec3 spec)
{
	vec3 L = normalize(-pl.direction.xyz);
	//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, 1.0);
	return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, 1.0, 0.0);
}

vec3 computeContributingSpotLights(SpotLight pl, vec3 No, vec3 Ca, vec3 Po, vec3 diff, vec3 spec)
{
	float a = acos(dot(normalize(pl.direction.xyz), normalize(Po - pl.position.xyz))) * (180.0 / M_PI);
	float attenuation = 0, distance = 0;
	vec3 Ia;
	distance = length(pl.position.xyz - Po);
	//attenuation = 1.0f / (pl.attenuation.x + pl.attenuation.y * distance + pl.attenuation.z * (distance * distance));
	//attenuation = 1.0f / (1.0f + distance * distance* pl.attenuation.x);
	if (a < pl.attenuation.w)
	{
		// Compute attenuation
		//compute the lenght of the vector
		vec3 L = normalize(pl.position.xyz - Po);
		//return Phong(vec3(pl.color.xyz), light.ambiant, light.diffuse, light.specular, L, No, Ca, diff, spec, attenuation);
		return BlinnPhong(vec3(pl.color.xyz), pl.K.xyz, L, No, Ca, diff, spec, attenuation, 0.0);
	}
	Ia = attenuation * light.ambiant * diff;
	return vec3(Ia);
}

/*
* Function to get the correct UVcoords from the parallax mapping according to the Viewing direction.
* heigthMap : sampler2D texture heightmap
* UVcoords : UV coordianates of the mesh
* Vdir : Viewing direction in the fragment space
* BParallax : Parameter to control the parallax mapping
*/
vec2 ParallaxMapping(sampler2D heightMap, vec2 UVcoords, vec3 Vdir, float bParallax)
{
	const float minLayers = 8;
	const float maxLayers = 32;
	float nbLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), Vdir)));
	//calculate the depth between each layer
	float offset = 1.0 / nbLayers;
	//begin by the first layer
	float currentDepthLayer = 0.0;

	//get the shift coordinates of the heightmap texture
	vec2 p = Vdir.xy / (Vdir.z + 0.05) * bParallax;
	vec2 deltaTexCoords = p / nbLayers;

	// get initial values
	vec2  currentTexCoords = UVcoords;
	float currentDepthMapValue = texture(heightMap, currentTexCoords).r;
	  
	while(currentDepthLayer < currentDepthMapValue)
	{
	    // shift texture coordinates along direction of P
	    currentTexCoords -= deltaTexCoords;
	    // get depthmap value at current texture coordinates
	    currentDepthMapValue = texture(heightMap, currentTexCoords).r;  
	    // next layer
	    currentDepthLayer += offset;  
	}


	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentDepthLayer;
	float beforeDepth = texture(heightMap, prevTexCoords).r - currentDepthLayer + offset;
 
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return currentTexCoords;
}