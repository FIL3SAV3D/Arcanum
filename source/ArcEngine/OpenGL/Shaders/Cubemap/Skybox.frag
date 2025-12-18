#version 460 core

in vec3 WorldPos;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    FragColor = textureLod(skybox, WorldPos, 2.0);
}