#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    TexCoord = vec2(vertex.z, vertex.w);
}