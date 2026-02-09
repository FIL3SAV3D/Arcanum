#version 450

layout(location = 0) out vec4 FragColor;

in PerVertexData
{
  vec3 color;
} fragIn;   

void main()
{
  uint gtid = 0;

  FragColor = vec4(fragIn.color, 1.0);
}