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

uniform mat4 M;
out vec3 UV;

void main()
{
	mat4 view = mat4(mat3(cam.V));
	gl_Position = (cam.P * view * vec4(Position, 1.0)).xyww;
	UV = Position;
}