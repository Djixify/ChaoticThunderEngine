#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec3 position;
} vs_out;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    vs_out.position = aPos;
}