#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform float time;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z + cos(aPos.x * 10 + time) + sin(aPos.y * 10 + time), 1.0);
}