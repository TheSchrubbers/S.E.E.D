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

out vec3 color;

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

void main()
{
	color = Color;
	gl_PointSize = 5;
	gl_Position = cam.P * cam.V * vec4(Position, 1.0);
}