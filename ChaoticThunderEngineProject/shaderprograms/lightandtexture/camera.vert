#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
	vec2 Tex;
    vec3 Normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Tex = aTex;
    vs_out.Normal = aNormal;
    gl_Position = projection * view * vec4(aPos, 1.0);
}

