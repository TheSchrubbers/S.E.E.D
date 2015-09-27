#version 440

#define MAX_NUM_TOTAL_LIGHTS 20

struct Light
{
	vec4 position;
	vec4 color;
	ivec4 size;
};

layout(std140, binding = 0) uniform LightsBuffer
{
	Light lights[MAX_NUM_TOTAL_LIGHTS];
};

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 V_inverse;
uniform mat4 Normal_Matrix;

out vec3 L[MAX_NUM_TOTAL_LIGHTS];
out vec3 C;
out vec3 N;
out vec2 UV;
out uint sizeLights;

void main()
{
	sizeLights = lights[0].size.x;
	vec3 Ptmp = (M * vec4(Position, 1.0)).xyz;
	vec3 Ltmp;
	for (int i = 0; i < lights[0].size.x; i++)
	{
		Ltmp = lights[i].position.xyz;
		L[i] = normalize(Ltmp - Ptmp);
	}
	vec3 Ctmp = (V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	//L = lights[0].position.xyz;
	C = normalize(Ptmp - Ctmp);
	N = normalize(mat3(Normal_Matrix) * Normal);
	//N = Normal;
	UV = UVcoord;


	gl_Position = MVP * vec4(Position, 1.0);
}