float processPerspectiveShadow(vec4 lightInSpace, sampler2D shadowTexture, float b);
float processOrthographicShadow(vec4 lightInSpace, sampler2D shadowTexture, float b);
float LinearizeDepth(float depth);

float processPerspectiveShadow(vec4 lightInSpace, sampler2D  gShadow, float b)
{
	float shadowDepth;
	vec3 l = lightInSpace.xyz / lightInSpace.w;
	l = l * 0.5 + 0.5;
	l.z = LinearizeDepth(l.z) / 30.0;

	float shadow = 0.0;

	if(l.z <= 1.0)
	{
		//size of a texel on the gshadowmaptexture
		vec2 texelSize = 1.0 / textureSize(gShadow, 0);
		//use BLUR to smooth shadow
		for(int i = -1; i <= 1; ++i)
		{
			for(int j = -1; j <= 1; ++j)
			{
				//corresponding depth point in shadowmap pass
				shadowDepth = texture(gShadow, l.xy + vec2(i, j) * texelSize).x;
				shadowDepth = LinearizeDepth(shadowDepth) / 30.0;
				if(shadowDepth < (l.z - b))
				{
					shadow += 1.0;
				}		
			}
		}
		shadow /= 9.0;
		//bias between 0.005 and 0.05
		//float bias = max(0.05 * (1.0 - dot(N, LShadow)), 0.005);
		
	}
	return shadow;
}

float processOrthographicShadow(vec4 lightInSpace, sampler2D gShadow, float b)
{
	float shadowDepth;
	vec3 l = lightInSpace.xyz / lightInSpace.w;
	l = l * 0.5 + 0.5;

	float shadow = 0.0;

	if(l.z <= 1.0)
	{
		//size of a texel on the gshadowmaptexture
		vec2 texelSize = 1.0 / textureSize(gShadow, 0);
		//use BLUR to smooth shadow
		for(int i = -1; i <= 1; ++i)
		{
			for(int j = -1; j <= 1; ++j)
			{
				//corresponding depth point in shadowmap pass
				shadowDepth = texture(gShadow, l.xy + vec2(i, j) * texelSize).x;
				if(shadowDepth < (l.z - b))
				{
					shadow += 1.0;
				}		
			}
		}
		shadow /= 9.0;
		//bias between 0.005 and 0.05
		//float bias = max(0.05 * (1.0 - dot(N, LShadow)), 0.005);
		
	}
	return shadow;
}


float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * 1.0 * 30.0) / (30.0 + 1.0 - z * (30.0 - 1.0));
}