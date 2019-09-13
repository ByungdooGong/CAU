#version 330 core
out vec4 color;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D albedo;

//Light Properties
uniform vec3 vPos;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform int sign;

//Spot Light
uniform vec3 spotDir;
uniform float cutOff;

void main()
{
	vec3 objectColor = vec3( 0.3f, 0.0f, 0.0f );
	vec3 lightDir = normalize(lightPosition - FragPos);
	float theta = dot(lightDir, normalize(-spotDir));

	if(theta > cutOff)
	{
		float ambientStrength = 0.5;
		vec3 ambient = ambientStrength * lightColor;
	
		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPosition - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		//attenuation
		if(sign==1)
		{
			float distance = length(lightPosition - FragPos);
			float r = 16.0f;
			float attenuation = 1.0f / (1 + (2.0f/r*distance) + (1.0f/(r*r)*(distance*distance)));
			ambient *= attenuation;
			diffuse *= attenuation;
		}
		vec3 result = (ambient+diffuse) * objectColor;
		color = vec4(result, 1.0f);
	}
	else
	{
		vec4 ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		color = vec4(ambient*vec4(objectColor, 1.0f));
	}
	
}