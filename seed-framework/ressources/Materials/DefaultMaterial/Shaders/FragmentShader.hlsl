#version 440

#define MAX_NUM_TOTAL_LIGHTS 20

in vec3 L[MAX_NUM_TOTAL_LIGHTS];
in vec3 C;
in vec3 N;
in vec2 UV;
flat in uint sizeLights;

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
	vec3 Id = vec3(0.0), Is = vec3(0.0);
	vec3 Li, R;
	for (int i = 0; i < sizeLights; i++)
	{
		Li = L[i];
		Id += (1/sizeLights) * light.diffuse * texcolor * max(dot(Li, N), 0);
		R = normalize(reflect(Li, N));
		Is += (1 / sizeLights) * light.specular * vec3(1.0) * pow(max(dot(R, C), 0), 10.0);
	}
	Color = vec4(Ia + Id + Is, 1.0);
	//Color = vec4(sizeLights);
	//Color = vec4(1.0,1.0,1.0,1.0);
}

/*Phong(vec3 P, vec3 L, vec3 V, vec3 color, float is, float id)
{
	vec3 vecL = normalize(L - P);
	return id * 
}*/