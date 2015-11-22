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

layout(location = 0) in vec3 Position;

uniform mat4 M;
out vec3 Po;

void main()
{
	Po = Position;
	mat4 orientation = mat4(1.0);
	for (int i = 0; i < 4; i++)
	{
		orientation[i][3] = cam.V[i][3];
		orientation[3][i] = cam.V[3][i];
	}
	gl_Position = cam.P * orientation * M * vec4(Position, 1.0);
	gl_PointSize = 0.8;
}