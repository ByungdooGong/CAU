#version 330 core

out vec4 color;

uniform vec3 inColor;
uniform float data;

void main()
{
	if(data < -0.07)
		discard;
	color = vec4(inColor.xyz, 1.0f);
}