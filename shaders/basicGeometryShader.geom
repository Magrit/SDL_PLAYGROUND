#version 330 core
layout (triangles, invocations = 1) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 model;
uniform mat4 proj;

in TE_OUT {
    vec4 color;
} gs_in[];

out GS_OUT{
  vec4 color;
  vec3 normal;
  vec3 position;
} gs_out;

vec3 getNormal(vec3 p0, vec3 p1, vec3 p2)
{
   vec3 a = p0 - p1;
   vec3 b = p2 - p1;
   return normalize(cross(a, b));
}

void main() {
    gs_out.color = gs_in[0].color;
    vec4 pos0 = proj*model*gl_in[0].gl_Position;
    vec4 pos1 = proj*model*gl_in[1].gl_Position;
    vec4 pos2 = proj*model*gl_in[2].gl_Position;
    gs_out.normal = getNormal(pos0.xyz, pos1.xyz, pos2.xyz);

    gs_out.position = pos0.xyz;
    gl_Position = pos0;
    EmitVertex();
    gs_out.position = pos1.xyz;
    gl_Position = pos1;
    EmitVertex();
    gs_out.position = pos2.xyz;
    gl_Position = pos2;
    EmitVertex();
    gl_Position = pos0;
    EmitVertex();

    EndPrimitive();
}
