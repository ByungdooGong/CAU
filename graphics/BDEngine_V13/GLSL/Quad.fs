#version 330 core

in vec3 Normal;
in vec2 texCoord;
in vec3 cam;

out vec4 color;

uniform sampler2D inTexture;
uniform vec3 camPos;

void main()
{

	color = texture(inTexture, texCoord);
	
}

	