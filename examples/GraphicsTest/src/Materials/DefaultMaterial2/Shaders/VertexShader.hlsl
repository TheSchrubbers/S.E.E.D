#version 440

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normals;
layout(location = 2) in vec3 Tangents;
layout(location = 3) in vec2 UVcoord;

uniform mat4 MVP;

out vec3 couleur;
out vec2 UV;

void main()
{
	//couleur = Couleur;
	//couleur = vec3(UVcoord, 0.0);
	gl_Position = MVP * vec4(Position, 1.0);
	UV = UVcoord;
}