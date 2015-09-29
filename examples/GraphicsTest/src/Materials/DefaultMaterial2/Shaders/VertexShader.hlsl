#version 440

struct Camera
{
	mat4 V;
	mat4 P;
	mat4 V_inverse;
};

layout(std140, binding = 1) uniform CameraBuffer
{
	Camera cam;
};
 
//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec3 Tangents;
layout(location = 3) in vec2 UVcoord;

void main()
{
	gl_Position = cam.P * cam.v * M * vec4(Position, 1.0);
}