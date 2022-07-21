#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform float time;
uniform mat4 view;



void main()
{
    gl_Position = projection * view * vec4(aPos.x + cos(time), aPos.y + sin(time), aPos.z, 1.0);
}


