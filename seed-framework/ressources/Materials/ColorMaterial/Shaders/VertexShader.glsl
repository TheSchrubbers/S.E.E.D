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

uniform mat4 M;
uniform mat4 Normal_Matrix;
uniform mat4 VPLight;

out vec3 N;
out vec2 UV;
out vec3 C;
out vec3 P;
out vec4 PInLightSpace;
out vec3 LShadow;

void main()
{	
	vec4 tmp;
	//Point to the world space
	tmp = M * vec4(Position, 1.0);
	P = tmp.xyz/tmp.w;

	//Normal in world space
	N = normalize(mat3(Normal_Matrix) * Normal);

	//Camera vector in the world space
	tmp = cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0);
	tmp /= tmp.w;
	C = normalize(P - tmp.xyz);

	//Point in Light space
	PInLightSpace = VPLight * M * vec4(Position, 1.0);

	LShadow = normalize(vec3(0.0, 3.0, 8.0) - P);

	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}