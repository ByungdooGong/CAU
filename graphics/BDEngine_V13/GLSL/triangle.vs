#version 330 core

layout (location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;

void main()
{
	gl_Position = proj * view * model * vec4(pos.xyz, 1.0);
	texCoord = tex;
	
}