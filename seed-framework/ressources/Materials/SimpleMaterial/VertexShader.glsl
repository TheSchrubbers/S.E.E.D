#version 440

#include "Lights.glsl"
#include "Camera.glsl"


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
out vec4 PInLightSpace[NB_SHADOW_TEXTURES];
out vec3 LShadow;
out float Attenuation;

void main()
{	
	gl_Position = cam.P * cam.V * M * vec4(Position, 1.0);
}
