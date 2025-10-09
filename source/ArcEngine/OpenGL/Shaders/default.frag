#version 330 core

uniform float opacity;

uniform sampler2D tex01;
uniform sampler2D tex02;


in vec4 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

void main()
{
   FragColor = mix(texture(tex01, texCoord), texture(tex02, texCoord), 0.2);
}