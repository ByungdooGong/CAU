#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 color;

uniform vec3 camPos;
uniform samplerCube skybox;
uniform sampler2D inTexture;
void main()
{
	vec3 I = normalize(FragPos - camPos);
	vec3 R = reflect(I, normalize(Normal));

	//color = mix(texture(inTexture, TexCoord), vec4(texture(skybox, R).rgb, 1.0), 0.5);
	color = mix(vec4(0.5f, 0.5f, 0.0f, 1.0f), vec4(texture(skybox, R).rgb, 1.0), 0.5);
}