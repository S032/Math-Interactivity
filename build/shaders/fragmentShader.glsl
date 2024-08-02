#version 330 core
in vec3 normal;
in vec3 vPos;

out vec4 Fragment;

uniform sampler2D Box;
uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float aChanel;
uniform float ambietStrength;

void main()
{
    vec3 _normal = normalize(normal);
    vec3 ambient = ambietStrength * lightColor;
    vec3 lightDir = normalize(lightPos - vPos);
    vec3 diffuse = max(dot(_normal, lightDir), 0.0) + 0.2 * lightColor;
    float specularStrength = 0.4;
    vec3 viewDir = normalize(camPos - vPos);
    vec3 reflectDir = reflect(-lightDir, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 res = (ambient + diffuse + specular) * color;

    Fragment = vec4(res, aChanel);
}