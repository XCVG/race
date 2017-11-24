#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D fBuffer; //normal framebuffer
uniform sampler2D sBuffer; //smear buffer
void main()
{
	color.rgb = texture(fBuffer, uv).rgb;
	color.a = 1.0;
}