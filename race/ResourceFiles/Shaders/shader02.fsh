#version 330 core
in vec2 uv;
out vec3 color;
uniform sampler2D fColor;
uniform sampler2D fPosition;
uniform sampler2D fNormal;
uniform sampler2D fDepth;
uniform sampler2D sDepth;
uniform vec3 ambientLight;
uniform vec3 directionalLight;
uniform mat4 biasMVP;
void main()
{
	vec3 position = texture( fPosition, uv).rgb;
	vec4 shadowCoord = biasMVP * vec4(position,1);
	
	float shadowValue =  texture( sDepth, shadowCoord.xy ).r ;
	
	float bias = 0.009;
	float visibility = 1.0;
	if ( shadowValue  <  shadowCoord.z-bias)
	{
		visibility = 0.0;
	}
	
	vec3 sDirectionalLight = directionalLight * visibility;
	
	color = texture(fColor, uv).rgb * (ambientLight + sDirectionalLight);  
	//color = shadowCoord.rgb;
	//color = texture( sDepth, uv).rgb;
}