#version 440

struct Camera
{
	mat4 V;
	mat4 P;
	mat4 V_inverse;
};

layout(std140, binding = 0) uniform CameraBuffer
{
	Camera cam;
};

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 M;
uniform mat4 Normal_Matrix;
uniform vec2 T;
uniform mat4 OrthoMatrix;
uniform mat4 LA;
out vec3 N;
out vec3 L;

void main()
{
	vec4 Ptmp = cam.V * M * vec4(Position, 1.0);
	Ptmp.xyz /= Ptmp.w;
	N = (cam.V * Normal_Matrix * vec4(Normal,0.0)).xyz;
	vec3 Lpos = vec3(0.0, 0.0, 0.0);
	L = normalize(Lpos - Ptmp.xyz);
	vec2 p = (cam.P * cam.V * M * vec4(Position, 1.0)).xy;
	p += T;
	gl_Position = vec4(p, 0.0,1.0);
}