#version 440

out vec4 Color;
uniform vec3 col;

in vec3 L;
in vec3 N;

void main()
{
	vec3 Ia = col * 0.3;
	vec3 Id = col * 0.4 * max(dot(L, N), 0.0);
	vec3 R = reflect(-L, N);
	//Color = vec4(Ia + Id + Is, 1.0);
	Color = vec4(Ia + Id, 1.0);
}