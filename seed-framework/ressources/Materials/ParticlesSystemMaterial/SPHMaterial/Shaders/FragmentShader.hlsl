#version 440

in vec3 C;
in vec3 P;
in vec3 N;
in vec3 L;
in vec3 col;

flat in uint sizeLights;

//OUT
out vec4 Color;

//FUNCTIONS
vec3 Phong(vec3 LColor, float Ka, float Kd, float Ks, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);

void main()
{
	vec3 Ia = 0.2 * col;
	vec3 Id = 0.4 * col * max(dot(L, N), 0.0);
	vec3 R = normalize(reflect(L, N));
	vec3 Is = 0.4 * vec3(1.0) * pow(max(dot(R, C), 0.0), 100.0);
	vec3 color = Ia + Id + Is;
	Color = vec4(color, 1.0);
}

//Phong : 
//vec3 color of the light -> LColor
//float coefficient ambiant -> Ka,
//float coefficient diffusion -> Kd,
//float coefficient specular -> Ks, 
//vec3 light -> L,
//vec3 Normal -> No,
//vec3 Camera -> Ca,
//vec3 diffMap -> diffMap,
//vec3 specMap -> specMap
vec3 Phong(vec3 LColor, float Ka, float Kd, float Ks, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation)
{
	//compute ambiant contributing
	vec3 Ia = Ka * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Kd * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Ks * pow(max(dot(R, Ca), 0), 10.0);
	return Ia + Is;
}