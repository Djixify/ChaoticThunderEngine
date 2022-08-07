#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


const float MAGNITUDE = 1.0;

uniform mat4 projection;
uniform mat4 view;

in VS_OUT {
    vec3 position;
} vs_in[];

out GS_OUT {
	vec3 normal;
} gs_out[];

vec3 GetNormal()
{
   vec3 a = vec3(vs_in[0].position) - vec3(vs_in[1].position);
   vec3 b = vec3(vs_in[2].position) - vec3(vs_in[1].position);
   return normalize(cross(a, b));
}

void main()
{
	vec4 _normal = projection * view * vec4(GetNormal(), 1.0);
    gl_Position = gl_in[0].gl_Position;
    gs_out[0].normal = _normal.xyz;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + _normal * MAGNITUDE;
    gs_out[0].normal = _normal.xyz;
    EmitVertex();
    EndPrimitive();
    
    gl_Position = gl_in[1].gl_Position;
    gs_out[1].normal = _normal.xyz;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position + _normal * MAGNITUDE;
    gs_out[1].normal = _normal.xyz;
    EmitVertex();
    EndPrimitive();
    
    gl_Position = gl_in[2].gl_Position;
    gs_out[2].normal = _normal.xyz;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + _normal * MAGNITUDE;
    gs_out[2].normal = _normal.xyz;
    EmitVertex();
    EndPrimitive();
}