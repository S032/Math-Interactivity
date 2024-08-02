#version 330 core
in vec3 aPos;
in vec3 aNorm;

out vec3 normal;
out vec3 vPos;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    normal = mat3(transpose(inverse(model))) * aNorm;
    vPos = aPos;
    gl_Position = mvp * vec4(aPos, 1.0);
}