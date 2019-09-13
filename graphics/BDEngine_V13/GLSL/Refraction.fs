#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

out vec4 color;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
	float ratio = 1.00 /1.52;
	vec3 I = normalize(FragPos - camPos);
	vec3 R = refract(I, normalize(Normal), ratio);

	//color = vec4(texture(skybox, R).rgb, 1.0);
	color = mix(vec4(0.5f, 0.0f, 0.5f, 1.0f), vec4(texture(skybox, R).rgb, 1.0), 0.5);
	
}