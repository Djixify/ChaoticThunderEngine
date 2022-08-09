#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

const float MAGNITUDE = 0.2;

uniform mat4 projection;

in VS_OUT {
	vec3 position;
} gs_in[];

out GS_OUT {
    vec3 position;
    vec3 normal;
} gs_out;

//out vec3 position;
//out vec3 normal;

vec3 GetNormal(vec3 v1, vec3 v2, vec3 v3)
{
   vec3 a = vec3(v1) - vec3(v2);
   vec3 b = vec3(v3) - vec3(v2);
   return normalize(cross(a, b));
}

void main()
{
    vec3 _normal = -GetNormal(gs_in[0].position, gs_in[1].position, gs_in[2].position);
    gl_Position = projection * gl_in[0].gl_Position;
    gs_out.position = gs_in[0].position;
    gs_out.normal = _normal;
    EmitVertex();
    gl_Position = projection * gl_in[1].gl_Position;
    gs_out.position = gs_in[1].position;
    gs_out.normal = _normal;
    EmitVertex();
    gl_Position = projection * gl_in[2].gl_Position;
    gs_out.position = gs_in[2].position;
    gs_out.normal = _normal;
    EmitVertex();
    EndPrimitive();
}