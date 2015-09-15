#version 430

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 Texcoord;

uniform mat4 MVP;

out vec3 couleur;
//out vec2 UV;

void main()
{
	couleur = vec3(1.0);
	gl_Position = MVP * vec4(Position, 1.0);
	//UV = UVcoord;
}