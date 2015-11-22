#version 440

//OUT
out vec4 Color;
in vec3 color;

void main()
{
		//final color
	Color = vec4(color, 1.0);
}

