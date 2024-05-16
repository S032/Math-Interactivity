#version 330 core
in vec2 texCord;
in vec3 normal;
in vec3 vPos;

out vec4 Fragment;

uniform sampler2D Box;
uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 objectPos;

void main()
{
    float ambietStrength = 0.1;
    vec3 ambient = ambietStrength * lightColor;
    vec3 lightDir = normalize(lightPos - objectPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor;
    float specularStrength = 0.4;
    vec3 viewDir = normalize(camPos - objectPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 res = (ambient + diffuse + specular) * color; 
    Fragment = vec4(res, 0.5);
}