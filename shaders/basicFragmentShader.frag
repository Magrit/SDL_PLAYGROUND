#version 330 core
in GS_OUT{
  vec4 color;
  vec3 normal;
  vec3 position;
} gs_out;

out vec4 color;
uniform vec3 timeVec;

void main()
{
  vec3 lightPos = vec3(sin(timeVec.y)*10.0f, sin(timeVec.y)*5.0f, cos(timeVec.y)*1.0f);
  vec3 norm = normalize(gs_out.normal);
  vec3 lightDir = normalize(lightPos-gs_out.position);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff*vec3(1.0, 1.0, 1.0);
  vec4 result = vec4(0.02f, 0.02f, 0.02f, 0.0f)+vec4(diffuse, 1.0f);
  color = gs_out.color*result;
}
