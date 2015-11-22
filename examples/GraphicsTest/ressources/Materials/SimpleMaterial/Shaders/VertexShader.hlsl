#version 440

struct Camera
{
	mat4 V;
	mat4 P;
	mat4 V_inverse;
};


struct Particle
{
	vec4 position;
	mat4 M;
	mat4 inverseM;
	vec4 color;
};
layout(std430, binding = 0) buffer ParticlesBuffer
{
	Particle particles[];
};

layout(std140, binding = 0) uniform CameraBuffer
{
	Camera cam;
};


//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

out vec3 N;
out vec2 UV;

out vec3 C;
out vec3 P;
out vec3 L;
out vec3 col;



void main()
{
	mat4 M = particles[gl_InstanceID].M;
	mat4 inverseM = particles[gl_InstanceID].inverseM;
	col = particles[gl_InstanceID].color.xyz;
	N = (M * vec4(Normal,0.0)).xyz;
	//set the coordinate point to the world space
	vec3 Ptmp = (M * vec4(Position, 1.0)).xyz;
	L = normalize(Position.xyz - vec3(0.0, 5.0, 5.0));
	//set camera vector to the world space
	vec3 Ctmp = (cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	C = normalize(Ptmp - Ctmp);
	P = Ptmp;
	
	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}