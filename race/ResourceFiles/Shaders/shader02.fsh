#version 330 core
in vec2 uv;
out vec3 color;
uniform sampler2D fColor;
uniform sampler2D fPosition;
uniform sampler2D fNormal;
uniform sampler2D fDepth;
void main()
{
   color = texture(fColor, uv).rgb;
}