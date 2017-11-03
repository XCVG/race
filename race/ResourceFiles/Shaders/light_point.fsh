#version 330 core
in vec2 uv;
out vec3 color;
uniform sampler2D fColor;
uniform sampler2D fPosition;
uniform sampler2D fNormal;
uniform sampler2D fDepth;
uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightRange;
uniform float lightIntensity;
void main()
{
	float smoothness = texture(fColor, uv).a;
	vec3 image = texture( fColor, uv).rgb;
    vec3 position = texture( fPosition, uv).rgb;
    vec3 normal = texture( fNormal, uv).rgb;
	
	vec3 lightDir = normalize(lightPos - position);
	normal = normalize(normal);
	
	vec3 eyeDir = normalize(cameraPos - position);
	vec3 vHalfVec = normalize(lightDir + eyeDir);
	
	//TODO add smoothness, range/fallloff, intensity mul, light color
	color =  max(dot(normal,lightDir),0) * image + pow(max(dot(normal,vHalfVec),0.0), 100) * 1.5;
}