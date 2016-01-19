#version 440

//vertexs
layout(location = 0) in vec3 Position;


out vec2 UV;

void main()
{	
	//set the vector camera, the position
	UV = Position.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
	gl_Position = vec4(Position.xy, 0.0, 1.0);
}