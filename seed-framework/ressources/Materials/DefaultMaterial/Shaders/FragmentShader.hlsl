#version 440

in vec3 L;
in vec3 C;
in vec3 N;
in vec2 UV;

out vec4 Color;

struct Light
{
	float ambiant;
	float diffuse;
	float specular;
};
uniform Light light;
uniform sampler2D samplerTexture;

void main()
{
	vec3 texcolor = vec3(texture(samplerTexture, UV));
	vec3 Ia = light.ambiant * texcolor;
	vec3 Id = light.diffuse * texcolor * max(dot(L, N), 0);
	vec3 R = normalize(reflect(L, N));
	vec3 Is = light.specular * vec3(1.0) * pow(max(dot(R, C), 0), 10.0);
	Color = vec4(Ia + Id + Is, 1.0);
	//Color = vec4(1.0);
	//Color = vec4(1.0,1.0,1.0,1.0);
}

/*Phong(vec3 P, vec3 L, vec3 V, vec3 color, float is, float id)
{
	vec3 vecL = normalize(L - P);
	return id * 
}*/