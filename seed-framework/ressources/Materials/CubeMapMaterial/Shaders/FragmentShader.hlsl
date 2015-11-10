#version 440

uniform samplerCube text;
out vec4 Color;
in vec3 UV;

void main()
{
	Color = texture(text, UV);
}