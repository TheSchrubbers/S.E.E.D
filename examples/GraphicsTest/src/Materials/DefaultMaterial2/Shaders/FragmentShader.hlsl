#version 440

out vec4 Color;
//in vec3 couleur;
in vec2 UV;

uniform sampler2D samplerTexture;

void main()
{
	Color = texture(samplerTexture, UV);
	//Color = vec4(couleur, 1.0);
	//Color = vec4(1.0,1.0,1.0,1.0);
}