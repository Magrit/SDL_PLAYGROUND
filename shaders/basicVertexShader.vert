#version 330 core
layout (location = 0) in vec3 position; // The position variable has attribute position 0

uniform vec3 timeVec;

out VS_OUT {
    vec4 color;
} vs_out;

out vec3 vPosition;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
void main()
{
mat4 rot = rotationMatrix(vec3(0.75,0.75,0.0),timeVec.y);
    vPosition = (rot*vec4(position, 1.0)).xyz;
    vs_out.color  = vec4(gl_VertexID*position.r, position.g, position.b, 1.0f);
    if(length(vs_out.color.rgb) < 1.01){
    vs_out.color.r = 0.1;
    }
}
