#version 440

#include "Camera.glsl"

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 M;
uniform mat4 Normal_Matrix;

out VERTEX_OUT
{
	vec3 PinWorldSpace;
	vec3 NinWorldSpace;
	vec3 VdirWorldSpace;

	vec3 PinFragSpace;
	vec3 VdirFragSpace;

	vec2 UV;

	mat3 TBN;
	mat3 transposeTBN;
}V_OUT;

void main()
{
	//compute TBN matrix
	vec3 T = normalize(mat3(Normal_Matrix) * Tangent);
	V_OUT.NinWorldSpace = normalize(mat3(Normal_Matrix) * Normal);
	vec3 B = cross(V_OUT.NinWorldSpace, T);
	//inverse of orthogonal matrices is its transpose
	V_OUT.TBN = mat3(T, B, V_OUT.NinWorldSpace);
	V_OUT.transposeTBN = transpose(V_OUT.TBN);
	
	//set the coordinate point to the world space
	V_OUT.PinWorldSpace = (M * vec4(Position, 1.0)).xyz;

	//set camera vector to the world space
	vec3 Ctmp = vec3(cam.V_inverse[3][0], cam.V_inverse[3][1], cam.V_inverse[3][2]);//
	//vec3 Ctmp = (cam.V_inverse * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	V_OUT.VdirWorldSpace = normalize(V_OUT.PinWorldSpace - Ctmp);

	//set the vector camera, the position
	V_OUT.UV = UVcoord;

	V_OUT.PinFragSpace = V_OUT.transposeTBN * V_OUT.PinWorldSpace;
	V_OUT.VdirFragSpace = normalize(-V_OUT.PinFragSpace + (V_OUT.transposeTBN * Ctmp));

	gl_Position = cam.P * cam.V * vec4(V_OUT.PinWorldSpace, 1.0);
}