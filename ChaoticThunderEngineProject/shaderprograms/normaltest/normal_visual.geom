#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

out VS_OUT {
    vec3 normal;
} vs_out;

const float MAGNITUDE = 0.2;

uniform mat4 projection;

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main()
{
    vs_out.normal = GetNormal();
    gl_Position = gl_in[0].gl_Position;
    
    EmitVertex();
}