#version 330 core

layout (location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;
void main()
{
	texCoord = tex;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	FragPos = vec3(model * vec4(pos, 1.0));

	gl_Position = proj * view * model * vec4(pos.xyz, 1.0);
		
}