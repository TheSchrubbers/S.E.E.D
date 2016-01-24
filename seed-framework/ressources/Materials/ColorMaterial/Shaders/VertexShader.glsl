#version 440

#define NB_SHADOW_TEXTURES 2
#define MAX_NUM_TOTAL_LIGHTS 3

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

struct Camera
{
	mat4 V;
	mat4 P;
	mat4 V_inverse;
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
layout(std140, binding = 4) uniform CameraBuffer
{
	Camera cam;
};


//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;

uniform mat4 M;
uniform mat4 Normal_Matrix;
uniform mat4 VPLight;

out vec3 N;
out vec2 UV;
out vec3 C;
out vec3 P;
out vec4 PInLightSpace[NB_SHADOW_TEXTURES];
out vec3 LShadow;
out float Attenuation;

void main()
{	
	vec4 tmp;
	int k = 0;
	//Point to the world space
	tmp = M * vec4(Position, 1.0);
	P = tmp.xyz/tmp.w;

	//Normal in world space
	N = normalize(mat3(Normal_Matrix) * Normal);

	//Camera vector in the world space
	tmp = cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0);
	tmp /= tmp.w;
	C = normalize(P - tmp.xyz);
	for (int i = 0; i < pointLights[0].size.x; i++)
	{
		PInLightSpace[k] = pointLights[i].VP * M * vec4(Position, 1.0);
		k++;
	}
	//process directionnalLights contributions
	for (int i = 0; i < directionnalLights[0].size.x; i++)
	{
		PInLightSpace[k] = directionnalLights[i].VP * M * vec4(Position, 1.0);
		k++;
	}
	//process spotLights contributions
	for (int i = 0; i < spotLights[0].size.x; i++)
	{
		PInLightSpace[k] = directionnalLights[i].VP * M * vec4(Position, 1.0);
		k++;
	}
	//Point in Light space
	//PInLightSpace = VPLight * M * vec4(Position, 1.0);

	LShadow = normalize(vec3(0.0, 3.0, 8.0) - P);

	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}
