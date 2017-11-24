#version 330 core
in vec2 uv;
uniform sampler2D fBuffer; //normal framebuffer
uniform sampler2D dBuffer; //depth buffer
uniform sampler2D sBuffer; //smear buffer
uniform float blurAmount;
uniform float dofAmount;
uniform float fogAmount;
void main()
{
	vec3 fColor = texture(fBuffer, uv).rgb;
	vec3 sColor = texture(sBuffer, uv).rgb;
	float depth = texture(dBuffer, uv).r;
	
	gl_FragColor.rgb = mix(fColor, sColor, blurAmount);
	gl_FragColor.a = 1.0;
	//gl_FragColor.rgb = sColor;
}