
vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation, float shadow);
vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation);

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
vec3 Phong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation, float shadow)
{
	//compute ambiant contributing
	vec3 Ia = Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	vec3 R = normalize(reflect(L, No));
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(R, Ca), 0), 10.0);
	return Ia + attenuation * (1.0 - shadow) * (Id + Is);
}

//Blinn	Phong : 
//vec3 color of the light -> LColor
//float coefficient ambiant -> Ka,
//float coefficient diffusion -> Kd,
//float coefficient specular -> Ks, 
//vec3 light -> L,
//vec3 Normal -> No,
//vec3 Camera -> Ca,
//vec3 diffMap -> diffMap,
//vec3 specMap -> specMap
vec3 BlinnPhong(vec3 LColor, vec3 Lcoef, vec3 L, vec3 No, vec3 Ca, vec3 diffMap, vec3 specMap, float attenuation, float shadow)
{
	vec3 H = normalize(L + Ca);
	//compute ambiant contributing
	vec3 Ia = Lcoef.x * diffMap;
	//compute diffuse contributing
	vec3 Id = LColor.xyz * Lcoef.y * diffMap * max(dot(L, No), 0);
	//compute specular contributing
	vec3 Is = LColor.xyz * Lcoef.z * specMap * pow(max(dot(No,H), 0.0), 10.0);
	return Ia + attenuation * (1.0 - shadow) * (Id + Is);
}