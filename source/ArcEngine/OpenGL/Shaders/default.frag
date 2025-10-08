#version 330 core

uniform float opacity;
uniform sampler2D ourTexture;

in vec4 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
   FragColor = texture(ourTexture, texCoord);
}