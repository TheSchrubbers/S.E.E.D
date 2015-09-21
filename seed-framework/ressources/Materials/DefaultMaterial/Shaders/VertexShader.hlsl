#version 440

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 V_inverse;
uniform mat4 Normal_Matrix;

out vec3 L;
out vec3 C;
out vec3 N;
out vec2 UV;

void main()
{
	vec3 Ltmp = vec3(0.0, 0.0, 5.0);
	vec3 Ptmp = (M * vec4(Position, 1.0)).xyz;
	vec3 Ctmp = (V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	L = normalize(Ltmp - Ptmp);
	C = normalize(Ptmp - Ctmp);
	N = normalize(mat3(Normal_Matrix) * Normal);
	//N = Normal;
	UV = UVcoord;


	gl_Position = MVP * vec4(Position, 1.0);
}