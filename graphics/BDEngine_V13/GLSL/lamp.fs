#version 330 core

in vec3 Normal;
in vec2 texCoord;
in vec3 FragPos;

out vec4 color;

//uniform sampler2D inTexture;
uniform sampler2D albedo;

//Light Properties
uniform vec3 vPos;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 solid;

//Spot Light
uniform vec3 spotDir;
uniform float cutOff;

void main()
{
	vec3 objectColor = vec3(0.5f, 0.5f, 0.0f);
	vec3 lightDir = normalize(lightPosition - FragPos);
	float theta = dot(lightDir, normalize(-spotDir));
	
	color = vec4(objectColor, 1.0);
	
}