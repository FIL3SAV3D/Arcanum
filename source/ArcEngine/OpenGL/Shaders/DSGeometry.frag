#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gDiffuse;
layout (location = 2) out vec3 gNormal;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_normal1;
// uniform sampler2D texture_ORM1;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(gNormal);
    gDiffuse = vec3(1.0, 0.5, 0.1);
}