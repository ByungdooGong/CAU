#version 330 core
out vec4 color;

in vec3 TexCoord;

uniform samplerCube cube_texture;

void main()
{
	color = texture(cube_texture, TexCoord);
}