#version 440

//vertexs
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSpec;

uniform sampler2D samplerAmbiantTexture0;
uniform sampler2D samplerSpecularTexture0;

in vec3 N;
in vec2 UV;
in vec3 C;
in vec3 P;

void main()
{
	gPosition = vec3(P);
	gNormal = normalize(N);
	gColorSpec.xyz = vec3(1.0);//texture(samplerAmbiantTexture0, UV).xyz;
	gColorSpec.w = 1.0;//texture(samplerSpecularTexture0, UV).x;
}