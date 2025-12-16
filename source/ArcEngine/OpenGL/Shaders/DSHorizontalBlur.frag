#version 460 core

in vec2 TexCoords;

uniform sampler2D customInput01;
uniform sampler2D customInput02;

layout (location = 0) out vec4 customOutput01;
layout (location = 1) out vec4 customOutput02;

void main()
{ 
    const float kernelSize = 30.0;
    const int halfSize = 10;
    const float coefficient = 1.0f / kernelSize;
    const vec2 dx = vec2(0.002, 0.0);

    customOutput02 = vec4(0.0);
    for(int x = -halfSize; x <= halfSize; x++)
    {
        customOutput02 += coefficient * texture(customInput02, TexCoords + x * dx);
    }

    customOutput01 = texture(customInput01, TexCoords);
}