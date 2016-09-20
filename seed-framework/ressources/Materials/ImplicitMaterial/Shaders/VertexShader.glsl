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
layout(location = 0) in vec3 Normal;

uniform mat4 M;
uniform mat4 Normal_Matrix;

out vec3 N;
out vec2 UV;

out vec3 C;
out vec3 P;


void main()
{
	//set the coordinate point to the world space
	vec3 Ptmp = (M * vec4(Position, 1.0)).xyz;
	N = normalize(mat3(Normal_Matrix) * Normal);
	//set camera vector to the world space
	vec3 Ctmp = (cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	C = normalize(Ptmp - Ctmp);

	P = Ptmp;
	gl_Position = cam.P * cam.V  * vec4(Position, 1.0);
}