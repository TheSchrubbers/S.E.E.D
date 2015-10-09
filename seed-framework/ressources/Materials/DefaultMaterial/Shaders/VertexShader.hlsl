#version 440

struct Camera
{
	mat4 V;
	mat4 P;
	mat4 V_inverse;
};

layout(std140, binding = 4) uniform CameraBuffer
{
	Camera cam;
};

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 M;
uniform mat4 Normal_Matrix;
out vec3 C;
out vec3 N;
out vec2 UV;
out vec3 P;

void main()
{
	vec3 Ptmp = (M * vec4(Position, 1.0)).xyz;
	vec3 Ctmp = (cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	C = normalize(Ptmp - Ctmp);
	N = normalize(mat3(Normal_Matrix) * Normal);
	UV = UVcoord;
	P = Ptmp;
	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}