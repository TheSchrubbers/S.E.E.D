#version 440

in vec3 Po;

//OUT
out vec4 Color;

void main()
{
	//final color
	Color = vec4(Po, 1.0);
}