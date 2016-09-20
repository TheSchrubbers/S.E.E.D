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
layout(location = 3) in vec2 UVcoords;

uniform mat4 M;
uniform mat4 Normal_Matrix;
uniform mat4 VLight;

out vec3 N;
out vec2 UV;

out vec3 C;
out vec3 P;
out vec3 LShadow;

out vec4 PInLightSpace;

void main()
{	
	//Point in light space
	vec4 tmp = VLight * M * vec4(Position, 1.0);
	PInLightSpace = tmp;

	//set the coordinate point to the world space
	tmp = M * vec4(Position, 1.0);
	P = tmp.xyz / tmp.w;

	//normal in world space
	N = normalize(mat3(Normal_Matrix) * Normal);

	//set camera vector to the world space
	tmp = (cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0));
	C = normalize(P - (tmp.xyz/tmp.w));

	UV = UVcoords;

	LShadow = normalize(vec3(0.0,3.0,8.0) - P);

	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}