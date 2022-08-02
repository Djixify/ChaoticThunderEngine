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

vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main()
{
    vec3 _normal = GetNormal();
    gl_Position = gl_in[0].gl_Position;
    gs_out.normal = _normal;
    //gs_out.normal = normalize(gl_in[0].gl_Position).xyz;
    gs_out.position = gl_in[0].gl_Position.xyz;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    gs_out.normal = _normal;
    //gs_out.normal = normalize(gl_in[1].gl_Position).xyz;
    gs_out.position = gl_in[1].gl_Position.xyz;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    gs_out.normal = _normal;
    //gs_out.normal = normalize(gl_in[2].gl_Position).xyz;
    gs_out.position = gl_in[2].gl_Position.xyz;
    EmitVertex();
    EndPrimitive();
}