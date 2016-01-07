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

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 M;
uniform mat4 Normal_Matrix;

out vec3 N;
out vec2 UV;
out vec3 C;
out vec3 P;

void main()
{
	//Position, normal, position of the camera in the screen space
	P = (cam.P * cam.V * vec4(Position, 1.0)).xyz;
	N = (cam.P * cam.V * vec4(Normal, 0.0)).xyz;
	C = (cam.V_inverse * vec4(0.0f,0.0f,0.0f,1.0f)).xyz;
	UV = UVcoord;
	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0f);
}