#version 330 core
out vec4 color;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D albedo;
uniform vec3 viewPos;
uniform vec3 spotDir[4];
uniform float cutOff;
uniform samplerCube skybox;

struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	

	//combine results
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * light.ambient ;

	//diffuse shading
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff *light.diffuse;
	
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = light.specular * spec;
	
	return (ambient + diffuse + specular) * 0.1 ;
}
struct PointLight{

	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform PointLight pLight[4];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 spotDir)
{
	float theta;
	vec3 lightDir = normalize(light.position - fragPos);
	theta = dot(lightDir, normalize(-spotDir));

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse ;
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 /(light.constant + light.linear*distance + light.quadratic * (distance * distance));
	//combine results
	float ambientStrength = 0.5;
	vec3 ambient = light.ambient * ambientStrength;
	
	vec3 specular = light.specular * spec ;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	if(theta>cutOff)
		return (ambient + diffuse + specular);
	else
		return ambient;
}

void main()
{
	vec3 objectColor = vec3( 1.0f, 1.0f, 0.0f );
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 R = reflect(viewDir, normalize(Normal));
	//vec3 result = CalcDirLight(dirLight, norm, viewDir);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	for(int i=0; i<4; i++)
		result += CalcPointLight(pLight[i], norm, FragPos, viewDir, spotDir[i]);
	result = result * objectColor;
	
	color = mix(vec4(result, 1.0), vec4(texture(skybox, R).rgb, 1.0), 0.2);
}