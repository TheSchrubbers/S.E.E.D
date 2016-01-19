#version 440

in vec2 UV;
out vec4 Color;

uniform sampler2D gShadowMap;

void main()
{
	float Depth = texture(gShadowMap, UV).x;
    //Depth = 1.0 - (1.0 - Depth) * 25.0;
    Color = vec4(vec3(Depth), 1.0);
}
