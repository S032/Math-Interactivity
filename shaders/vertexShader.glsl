#version 330 core
in vec3 aPos;
in vec2 aTex;
in vec3 aNorm;

out vec2 texCord;
out vec3 normal;
out vec3 vPos;

uniform mat4 mvp;

void main()
{
    texCord = aTex;
    normal = aNorm;
    vPos = aPos;
    gl_Position = mvp * vec4(aPos, 1.0);
}