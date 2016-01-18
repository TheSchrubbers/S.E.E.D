#version 440

//vertexs
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Tangent;
layout(location = 3) in vec2 UVcoord;

uniform mat4 WVP;

out vec2 UV;

void main()
{	
	UV = UVcoord;
	gl_Position = WVP * vec4(Position, 1.0);
}