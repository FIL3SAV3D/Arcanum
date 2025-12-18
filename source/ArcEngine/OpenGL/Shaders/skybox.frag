#version 460 core

in vec3 localPos;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{    
    FragColor = texture(skybox, localPos);
}