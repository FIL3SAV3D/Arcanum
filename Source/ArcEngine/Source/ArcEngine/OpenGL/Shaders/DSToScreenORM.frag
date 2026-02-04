#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gORM;

void main()
{ 
    FragColor = vec4(texture(gORM, TexCoords).rgb, 1.0);
}