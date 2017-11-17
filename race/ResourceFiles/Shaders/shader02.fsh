#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D fColor;
uniform sampler2D fPosition;
uniform sampler2D fNormal;
uniform sampler2D fDepth;
uniform sampler2D sDepth;
uniform vec3 clearColor;
uniform vec3 ambientLight;
uniform vec3 dLightColor;
uniform mat4 biasMVP;
void main()
{
	vec3 position = texture( fPosition, uv).rgb;
	vec4 shadowCoord = biasMVP * vec4(position,1);
	
	float shadowValue =  texture( sDepth, shadowCoord.xy ).r ;
	
	float bias = 0.01;
	float visibility = 1.0;
	if ( shadowValue  <  shadowCoord.z-bias)
	{
		visibility = 0.0;
	}
	
	vec3 sDirectionalLight = dLightColor * visibility;
	
	//shader-based clear color necessitated by the way we're doing deferred rendering
	float depth = texture(fDepth, uv).r;
	float cVisibility = 0.0;
	if(depth >= 1.0)
		cVisibility = 1.0;
	
	vec3 bgColor = clearColor * cVisibility;
	
	color.rgb = texture(fColor, uv).rgb * (ambientLight + sDirectionalLight) + bgColor;
	color.a = 1.0-cVisibility;
}