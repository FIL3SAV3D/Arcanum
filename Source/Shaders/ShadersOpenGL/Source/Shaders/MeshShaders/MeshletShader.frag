#version 450

layout(location = 0) out vec4 FragColor;

in VertexData{
    vec3 color;
} PerVertexData;

void main()
{
  FragColor = vec4(PerVertexData.color, 1.0);
}