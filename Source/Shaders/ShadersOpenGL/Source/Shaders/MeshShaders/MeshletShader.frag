#version 450

layout(location = 0) out vec4 FragColor;

in VertexData
{
  vec3 color;
} fragIn;   

void main()
{
  FragColor = vec4(fragIn.color, 1.0);
}