#version 330 core
out vec4 color;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{

  vec4 sampled = vec4(0xDD, 0x00, 0xDD, 
  texture(ourTexture, TexCoord).r);
  color = vec4(0.0, 0.0, 0.0, 0.0)+sampled;
}