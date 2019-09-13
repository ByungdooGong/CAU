#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 LocalPos;

out vec4 color;

uniform vec3 viewPos;
//uniform sampler2D albedo;

void main()
{
	float distance = length(LocalPos);
	float alpha = 1.0 / (1.0 + distance*7);
	
	color = vec4(1.0, 1.0, 0.0 , alpha);	
	
}