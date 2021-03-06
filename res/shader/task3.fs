#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specularStrength;

void main()
{
	//环境光照
    vec3 ambient = ambientStrength * lightColor;
	//漫反射光照
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	//镜面光照
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor; 
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
}