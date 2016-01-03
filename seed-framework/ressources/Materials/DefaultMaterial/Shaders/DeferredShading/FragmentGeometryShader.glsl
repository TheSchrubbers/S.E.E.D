#version 440

//vertexs
layout(location = 0) in vec3 gPosition;
layout(location = 1) in vec3 gNormal;
layout(location = 2) in vec3 gColorSpec;

uniform sampler2D samplerAmbiantTexture0;
uniform sampler2D samplerSpecularTexture0;

in vec3 N;
in vec2 UV;
in vec3 C;
in vec3 P;

void main()
{
	gPosition = P;
	gNormal = N;
	gColorSpec.xyz = texture(samplerAmbiantTexture0, UV).xyz;
	gColorSpec.w = texture(samplerSpecularTexture0, UV).x;
}